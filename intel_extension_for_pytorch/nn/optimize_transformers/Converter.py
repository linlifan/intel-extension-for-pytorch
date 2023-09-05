import torch 
import torch.nn
from typing import Optional
from .TensorSlicer import TensorSlicer
from .WeightLoader import WeightLoader
from .ModuleReplacer import ModuleReplacer
from .modules.Layers import EnvParam
from .modules._transformers import IPEXTransformerConverter
import torch.distributed as dist


import torch
from typing import Callable
from torch import Tensor
from packaging import version as pkg_version


class OnDevice(object):
    """
    Create modules/tensors w. specific devices and dtypes. Examples:

    Create MyModule which consists of many different sub-modules and parameters. In this case we can create
    MyModule as a collection of 'meta' tensors by passing `device='meta'` or we can create the module _directly_
    on a CUDA device by passing `device=f'cuda:{local_rank}'` (where `local_rank` is the local GPU id.

    with OnDevice(dtype=torch.float16, device='meta'):
        model = MyModel()

    with OnDevice(dtype=torch.float16, device=f'cuda:{local_rank}'):
        model = MyModel()

    """

    _orig_torch_empty = torch.empty
    _orig_torch_zeros = torch.zeros
    _orig_torch_ones = torch.ones
    _orig_torch_full = torch.full

    def __init__(self, dtype, device="meta", enabled=True):
        self.dtype = dtype
        self.enabled = enabled
        self.device = device

        if device == "meta":
            if pkg_version.parse('1.10') > pkg_version.parse(torch.__version__):
                raise NotImplementedError("Meta tensor support is not available, please upgrade to torch 1.10+")

    def fp_tensor_constructor(self, fn: Callable, target_fp_dtype: torch.dtype) -> Callable:

        def wrapped_fn(*args, **kwargs) -> Tensor:
            if kwargs.get("device", None) is None:
                kwargs['device'] = self.device
            tensor: Tensor = fn(*args, **kwargs)
            if tensor.is_floating_point():
                tensor = tensor.to(target_fp_dtype)
            return tensor

        return wrapped_fn

    def get_new_tensor_fn_for_dtype(self, dtype: torch.dtype) -> Callable:

        def new_tensor(cls, *args) -> Tensor:
            tensor = OnDevice._orig_torch_empty(0, device=self.device).new_empty(*args)
            if tensor.is_floating_point():
                tensor = tensor.to(dtype)
            return tensor

        return new_tensor

    def __enter__(self):
        if not self.enabled:
            return
        torch.Tensor.__old_new__ = torch.Tensor.__new__
        torch.Tensor.__new__ = self.get_new_tensor_fn_for_dtype(self.dtype)
        torch.empty = self.fp_tensor_constructor(self._orig_torch_empty, self.dtype)
        torch.zeros = self.fp_tensor_constructor(self._orig_torch_zeros, self.dtype)
        torch.ones = self.fp_tensor_constructor(self._orig_torch_ones, self.dtype)
        torch.full = self.fp_tensor_constructor(self._orig_torch_full, self.dtype)

    def __exit__(self, exc_type, exc_value, traceback):
        if not self.enabled:
            return
        torch.Tensor.__new__ = torch.Tensor.__old_new__
        torch.empty = self._orig_torch_empty
        torch.zeros = self._orig_torch_zeros
        torch.ones = self._orig_torch_ones
        torch.full = self._orig_torch_full

def ds_kernel_injection_enabled(model, enable_ds):
    if not enable_ds:
        return False
    import deepspeed
    if isinstance(model, deepspeed.InferenceEngine):
        config = model._config
        ds_kernel_injection = config.replace_with_kernel_inject
        if ds_kernel_injection:
            return True
    return False

def ds_autotp_enabled(model, enable_ds=False):
    if not enable_ds:
        return False
    import deepspeed
    if isinstance(model, deepspeed.InferenceEngine):
        config = model._config
        tp_size = config.tensor_parallel.tp_size
        tp_group = config.tensor_parallel.tp_group
        IPEXTransformerConverter.update_tp_data(tp_size, tp_group)
        if tp_size > 1 and tp_group is not None:
            return True
    return False

class Converter:
    def __init__(self, ckpt = None, tp_fn = None, replaced_module = None, replaced_layer = None, replace_fn = None, distributed = False) -> None:
        self.tp_size = 1
        self.distributed = distributed
        if distributed:
            self.tp_size = dist.get_world_size()
        self.create_model_parallel_group()
        IPEXTransformerConverter.update_tp_data(self.tp_size, self.tp_group)
        self.tensor_slicer = TensorSlicer(self.tp_size, self.tp_group, tp_fn)
        self.module_replacer = ModuleReplacer(replaced_module, replaced_layer, replace_fn)
        self.weight_loader = WeightLoader(ckpt, self.tp_group, self.tp_size)


    def create_model_parallel_group(self):
        if self.tp_size < 2:
            self.tp_group = None
            return 
        if not hasattr(EnvParam, "tp_group") or getattr(EnvParam, "tp_group") is None:
            # init_distributed()
            ranks = [i for i in range(self.tp_size)]
            tp_group = dist.new_group(ranks)
            # EnvParam.set_env("tp_group", tp_group)
            self.tp_group = tp_group
        else:
            self.tp_group = EnvParam.tp_group

    def convert_model(self, model, dtype):
        try:
            import transformers
        except ImportError as e:
            print("Warning: we didn't find Huggingface's transformers package in your environments, please install it first, otherwise no feature of optimize_transformers will work")
            return model
        enable_ds = False
        try:
            import deepspeed
        except ImportError as e: ""
        else:
            if isinstance(model, deepspeed.InferenceEngine):
                enable_ds = True
                self.module_replacer.update_deepspeed_supported_op()

        if ds_kernel_injection_enabled(model, enable_ds):
            assert False, "Deepspeed's kernel injection is not supported in IPEX, please turn off the kernel injection option in Deepspeed and re-run the script."
        if not ds_autotp_enabled(model, enable_ds):
            model = self.tensor_slicer.slicing_model(model)
            self.weight_loader.load_weight_if_necessary(model)
            if self.distributed:
                device = "xpu:{}".format(dist.get_rank())
            else:
                device = "xpu"
            model = model.to(device).to(dtype)
        self.module_replacer.replace_func(model)
        self.module_replacer.replace_module(model, dtype, config=None)

        self.module_replacer.replace_op(model)

        return model

