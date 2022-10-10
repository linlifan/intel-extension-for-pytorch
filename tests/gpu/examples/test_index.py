import torch
from torch.testing._internal.common_utils import TestCase

import intel_extension_for_pytorch # noqa

import numpy as np

np.set_printoptions(threshold=np.inf)

cpu_device = torch.device("cpu")
dpcpp_device = torch.device("xpu")


class TestTorchMethod(TestCase):
    def test_index(self, dtype=torch.float):
        dtypes = [torch.float, torch.half]
        li = torch.tensor([i for i in range(27) for j in range(i)], device=cpu_device)
        lj = torch.tensor([j for i in range(27) for j in range(i)], device=cpu_device)

        for datatype in dtypes:
            # 2-dims tensor index
            x_cpu = torch.randn([327, 27], dtype=torch.float, device=cpu_device)
            if datatype == torch.half:
                x_cpu = x_cpu.half()

            res_cpu = x_cpu[:, lj]
            x_dpcpp = x_cpu.to(dpcpp_device)
            res_dpcpp = x_dpcpp[:, lj]
            self.assertEqual(res_cpu, res_dpcpp.to(cpu_device))

            # 3-dims tensor index
            x_cpu = torch.randn([327, 27, 27], dtype=torch.float, device=cpu_device)
            if datatype == torch.half:
                x_cpu = x_cpu.half()

            res_cpu = x_cpu[:, li, lj]
            x_dpcpp = x_cpu.to(dpcpp_device)
            res_dpcpp = x_dpcpp[:, li, lj]
            self.assertEqual(res_cpu, res_dpcpp.to(cpu_device))

            res_cpu = x_cpu[:, :, lj]
            x_dpcpp = x_cpu.to(dpcpp_device)
            res_dpcpp = x_dpcpp[:, :, lj]
            self.assertEqual(res_cpu, res_dpcpp.to(cpu_device))

            # 4-dims tensor index
            x_cpu = torch.randn([327, 27, 27, 27], dtype=torch.float, device=cpu_device)
            if datatype == torch.half:
                x_cpu = x_cpu.half()

            res_cpu = x_cpu[:, li, lj, lj]
            x_dpcpp = x_cpu.to(dpcpp_device)
            res_dpcpp = x_dpcpp[:, li, lj, lj]
            self.assertEqual(res_cpu, res_dpcpp.to(cpu_device))

            res_cpu = x_cpu[:, :, li, lj]
            x_dpcpp = x_cpu.to(dpcpp_device)
            res_dpcpp = x_dpcpp[:, :, li, lj]
            self.assertEqual(res_cpu, res_dpcpp.to(cpu_device))

    def test_index_of_bool_mask(self, dtype=torch.float):
        a_cpu = torch.randn(4, 15000, dtype=dtype)
        b_cpu = torch.randn(4, 15000, dtype=dtype)
        mask_cpu = a_cpu < b_cpu

        a_xpu = a_cpu.xpu()
        mask_xpu = mask_cpu.xpu()

        output_cpu = a_cpu[mask_cpu]
        output_xpu = a_xpu[mask_xpu]

        self.assertEqual(output_cpu, output_xpu)