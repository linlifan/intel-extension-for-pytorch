import torch
from torch.testing._internal.common_utils import (TestCase,
                                                  repeat_test_for_types)

from torch.testing._internal.common_dtype import (get_all_int_dtypes,
                                                  get_all_fp_dtypes)
import intel_extension_for_pytorch  # noqa
import pytest

cpu_device = torch.device("cpu")
xpu_device = torch.device("xpu")


class TestTorchMethod(TestCase):
    @repeat_test_for_types([*(get_all_int_dtypes() + get_all_fp_dtypes())])
    @pytest.mark.skipif(not torch.xpu.utils.has_fp64_dtype(), reason="fp64 not support by this device")
    def test_index_copy_dim_0(self, dtype):
        x = torch.ones([5, 3], device=cpu_device, dtype=dtype)
        t = torch.tensor([[1, 2, 3], [4, 5, 6], [7, 8, 9]], dtype=dtype)
        index = torch.tensor([0, 4, 2])
        x.index_copy_(0, index, t)

        x_xpu = torch.ones([5, 3], device=xpu_device, dtype=dtype)
        index_xpu = torch.tensor([0, 4, 2], device=xpu_device)
        t_d = t.to("xpu")
        x_xpu.index_copy_(0, index_xpu, t_d)

        self.assertEqual(x, x_xpu.to(cpu_device))

    @repeat_test_for_types([*(get_all_int_dtypes() + get_all_fp_dtypes())])
    @pytest.mark.skipif(not torch.xpu.utils.has_fp64_dtype(), reason="fp64 not support by this device")
    def test_index_copy_dim_1(self, dtype):
        x = torch.zeros([3, 5], device=cpu_device, dtype=dtype)
        t = torch.tensor([[1, 2, 3], [6, 7, 8], [11, 12, 13]], dtype=dtype)
        index = torch.tensor([0, 4, 2])
        x.index_copy_(1, index, t)

        x_xpu = torch.zeros([3, 5], device=xpu_device, dtype=dtype)
        index_xpu = torch.tensor([0, 4, 2], device=xpu_device)
        t_d = t.to("xpu")
        x_xpu.index_copy_(1, index_xpu, t_d).cpu()

        self.assertEqual(x, x_xpu.to(cpu_device))

    @repeat_test_for_types([*(get_all_int_dtypes() + get_all_fp_dtypes())])
    @pytest.mark.skipif(not torch.xpu.utils.has_fp64_dtype(), reason="fp64 not support by this device")
    def test_index_copy_multi_dim(self, dtype):
        # dim = 0
        x = torch.zeros([100, 3, 5], device=cpu_device, dtype=dtype)
        t = torch.tensor([[[1, 2, 3, 4, 5], [4, 5, 6, 7, 8], [9, 10, 11, 12, 13]],
                          [[11, 22, 33, 44, 55], [44, 55, 66, 77, 88], [99, 10, 11, 12, 13]],
                          [[111, 222, 333, 444, 555], [444, 555, 666, 777, 888], [999, 10, 11, 12, 13]]], dtype=dtype)
        index = torch.tensor([0, 66, 88])
        x.index_copy_(0, index, t)

        x_xpu = torch.zeros([100, 3, 5], device=xpu_device, dtype=dtype)
        index_xpu = torch.tensor([0, 66, 88], device=xpu_device)
        t_d = t.to("xpu")
        x_xpu.index_copy_(0, index_xpu, t_d)

        self.assertEqual(x, x_xpu.to(cpu_device))

        # dim = 1
        x = torch.zeros([3, 100, 5], device=cpu_device, dtype=dtype)
        t = torch.tensor([[[1, 2, 3, 4, 5], [4, 5, 6, 7, 8], [9, 10, 11, 12, 13]],
                          [[11, 22, 33, 44, 55], [44, 55, 66, 77, 88], [99, 10, 11, 12, 13]],
                          [[111, 222, 333, 444, 555], [444, 555, 666, 777, 888], [999, 10, 11, 12, 13]]], dtype=dtype)
        index = torch.tensor([0, 66, 88])
        x.index_copy_(1, index, t)
        # print(x)

        x_xpu = torch.zeros([3, 100, 5], device=xpu_device, dtype=dtype)
        index_xpu = torch.tensor([0, 66, 88], device=xpu_device)
        t_d = t.to("xpu")
        x_xpu.index_copy_(1, index_xpu, t_d)
        # print(x_xpu.cpu())

        self.assertEqual(x, x_xpu.to(cpu_device))

        # dim = 2
        x = torch.zeros([3, 5, 100], device=cpu_device, dtype=dtype)
        t = torch.tensor([[[1, 2, 3], [4, 5, 6], [9, 10, 11], [11, 22, 33], [44, 55, 66]],
                          [[99, 10, 11], [111, 222, 333], [444, 555, 666], [999, 10, 11], [1111, 2222, 3333]],
                          [[4444, 5555, 6666], [9999, 10, 11], [11111, 22222, 33333], [44444, 55555, 66666],
                           [99999, 10, 11]]], dtype=dtype)
        index = torch.tensor([0, 66, 88])
        x.index_copy_(2, index, t)

        x_xpu = torch.zeros([3, 5, 100], device=xpu_device, dtype=dtype)
        index_xpu = torch.tensor([0, 66, 88], device=xpu_device)
        t_d = t.to("xpu")
        x_xpu.index_copy_(2, index_xpu, t_d)

        self.assertEqual(x, x_xpu.to(cpu_device))