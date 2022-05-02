#include <ATen/ATen.h>

#include <ATen/core/TensorBody.h>
#include <ATen/native/TensorIterator.h>
#include <ATen/quantized/QTensorImpl.h>
#include <ATen/quantized/Quantizer.h>

#include <ATen/native/Resize.h>
#include <c10/core/ScalarType.h>
#include <core/Event.h>
#include <core/Guard.h>
#include <core/Memory.h>
#include <core/Stream.h>
#include <core/detail/TensorInfo.h>
#include <runtime/Exception.h>
#include <runtime/Utils.h>

#include "comm/ATDispatch.h"
#include "comm/RegistrationDeclarations.h"

#include <oneDNN/Utils.h>
#include <oneDNN/oneDNN.h>
#include "Loops.h"

using namespace at;
using namespace xpu::dpcpp;

namespace at {
namespace impl {

#define BUILD_TENSOR_ITER(dst, src, iter)       \
  auto iter = TensorIteratorConfig()            \
                  .set_check_mem_overlap(true)  \
                  .add_output(self)             \
                  .add_input(src)               \
                  .resize_outputs(false)        \
                  .check_all_same_dtype(false)  \
                  .check_all_same_device(false) \
                  .build();

static bool copy_requires_temporaries(TensorIterator& iter, bool p2p_enabled) {
  Device dst_device = iter.device(0);
  Device src_device = iter.device(1);

  if (dst_device == src_device) {
    // We never require temporaries for copies on the same GPU.
    TORCH_INTERNAL_ASSERT(
        dst_device.type() == c10::DeviceType::XPU &&
        src_device.type() == c10::DeviceType::XPU);
    return false;
  }

  bool same_dtype = iter.dtype(0) == iter.dtype(1);
  if (same_dtype && iter.is_contiguous()) {
    // Contiguous same-dtype copies can always use sycl copy
    return false;
  } else if (
      dst_device.type() == c10::DeviceType::XPU &&
      src_device.type() == c10::DeviceType::XPU) {
    // Copies between GPUs can use the copy kernel if P2P is supported
    return !p2p_enabled;
  } else {
    // The remaining cases require temporaries. For example, this includes
    // non-contiguous copies between CPU and GPU.
    return true;
  }
}

static bool maybe_enable_p2p_access(Device dst_device, Device src_device) {
  if (dst_device.is_cpu() || src_device.is_cpu()) {
    return false;
  }
  // no p2p so far
  return false;
}

void copy_device_to_device(TensorIterator& iter, bool non_blocking) {
  auto numel = iter.numel();
  if (numel == 0) {
    return;
  }

  // We can memcpy the memory if both tensors have the same type AND both
  // tensors are contiguous after dimension coalescing and reordering.
  bool same_type = iter.dtype(0) == iter.dtype(1);
  bool same_conj = iter.tensor(0).is_conj() == iter.tensor(1).is_conj();
  bool same_neg = iter.tensor(0).is_neg() == iter.tensor(1).is_neg();
  bool memcpy_eligible =
      same_type && same_conj && same_neg && iter.is_contiguous();

  Device dst_device = iter.device(0);
  Device src_device = iter.device(1);

  // We always perform the copy on the source device, using the current stream
  // on the source device, and we fully synchronize on both src and dst's
  // current streams for completion of the copy.
  DPCPPGuard device_guard(src_device);
  DPCPPStream copy_stream = getCurrentDPCPPStream(src_device.index());
  if (src_device != dst_device) {
    // This is a cross-device copy on the src current stream and dst current
    // stream. We perform a two-way barrier between both devices' streams
    // before the copy. This ensures that any write-after-write and
    // write-after-read dependencies on the destination side are handled, so
    // that no one is operating on the dst memory when we perform the copy.
    // src waits on dst barrier (src already waits on src)
    DPCPPEvent dst_ready;
    dst_ready.record(getCurrentDPCPPStream(dst_device.index()));

    dst_ready.block(copy_stream);
  }

  if (memcpy_eligible) {
    dpcppMemcpyAsync(
        iter.data_ptr(0),
        iter.data_ptr(1),
        numel * iter.element_size(0),
        DeviceToDevice);
  } else {
    auto dtype = iter.dtype(0);
    if (isQIntType(dtype)) {
      IPEX_DISPATCH_QINT_TYPES(dtype, "copy_", [&] {
        dpcpp_kernel_for_tensor_iter(
            iter, [=](scalar_t src_val) { return src_val; });
      });
    } else {
      if (same_neg) {
        if (!same_conj && same_type) {
          IPEX_DISPATCH_COMPLEX_TYPES(dtype, "copy_conj", [&] {
            dpcpp_kernel_for_tensor_iter(
                iter, [=](scalar_t src_val) { return std::conj(src_val); });
          });
        } else {
          IPEX_DISPATCH_ALL_TYPES_AND_COMPLEX_AND3(
              kBool, kHalf, kBFloat16, dtype, "copy_", [&] {
                dpcpp_kernel_for_tensor_iter(
                    iter, [=](scalar_t src_val) { return src_val; });
              });
        }
      } else {
        if (!same_conj && same_type) {
          IPEX_DISPATCH_COMPLEX_TYPES(dtype, "copy_conj", [&] {
            dpcpp_kernel_for_tensor_iter(
                iter, [=](scalar_t src_val) { return std::conj(-src_val); });
          });
        } else {
          IPEX_DISPATCH_ALL_TYPES_AND_COMPLEX_AND3(
              kBool, kHalf, kBFloat16, dtype, "copy_", [&] {
                dpcpp_kernel_for_tensor_iter(
                    iter, [=](scalar_t src_val) { return -src_val; });
              });
        }
      }
    }
  }

  if (src_device != dst_device) {
    // dst waits on src barrier (dst already waits on dst). We cannot
    // operate on dst's copy until the copy is complete.
    // Still on src_device, record stream event
    DPCPPEvent src_ready;
    src_ready.record(copy_stream);

    src_ready.block(getCurrentDPCPPStream(dst_device.index()));
  }
}

Tensor as_strided_quantized_dpcpp(
    const Tensor& self,
    IntArrayRef size,
    IntArrayRef stride) {
  auto storage_offset = self.storage_offset();
  auto quantizer = at::get_qtensorimpl(self)->quantizer();
  auto result = detail::make_tensor<QTensorImpl>(
      Storage(self.storage()), self.key_set(), self.dtype(), quantizer);
  native::setStrided(result, size, stride, storage_offset);
  return result;
}

Tensor expand_as_quantized_dpcpp(const Tensor& self, const Tensor& other) {
  auto size = other.sizes();
  TORCH_CHECK(
      size.size() >= (size_t)self.dim(),
      "expand(",
      self.toString(),
      "{",
      self.sizes(),
      "}, size=",
      size,
      "): the number of sizes provided (",
      size.size(),
      ") ",
      "must be greater or equal to the number of dimensions in the tensor (",
      self.dim(),
      ")");

  std::vector<int64_t> expandedSizes;
  std::vector<int64_t> expandedStrides;
  std::tie(expandedSizes, expandedStrides) =
      inferExpandGeometry(self.sizes(), self.strides(), size);

  auto result =
      as_strided_quantized_dpcpp(self, expandedSizes, expandedStrides);
#ifdef BUILD_NAMEDTENSOR
  namedinference::propagate_names_for_expand(result, self);
#endif
  return result;
}

void copy_kernel_dpcpp(TensorIterator& iter, bool non_blocking) {
  AT_ASSERT(iter.ntensors() == 2);

  Device dst_device = iter.device(0);
  Device src_device = iter.device(1);

  // Enable p2p access between devices. (No-op if it invovles the CPU)
  bool p2p_enabled = maybe_enable_p2p_access(dst_device, src_device);

  if (copy_requires_temporaries(iter, p2p_enabled)) {
    // NB: this involves recursive calls to copy. Be careful that those copies
    // don't require temporaries or you will cause an infinite recursion!
    auto& dst = iter.tensor(0);
    Tensor dst_contig;
    Tensor src_contig;

    // Type conversions are performed on the CPU for CPU-GPU copies and on
    // the src device for GPU-GPU copies.
    if (iter.device_type(0) == kXPU) {
      dst_contig = dst.is_contiguous()
          ? dst
          : at::empty_like(dst, LEGACY_CONTIGUOUS_MEMORY_FORMAT);
      if (dst.is_quantized()) {
        src_contig =
            expand_as_quantized_dpcpp(iter.tensor(1).to(iter.dtype(0)), dst)
                .contiguous();
      } else {
        src_contig =
            iter.tensor(1).to(iter.dtype(0)).expand_as(dst).contiguous();
      }
    } else {
      bool same_type = iter.dtype(0) == iter.dtype(1);
      dst_contig = (dst.is_contiguous() && same_type)
          ? dst
          : at::empty_like(dst, iter.dtype(1), LEGACY_CONTIGUOUS_MEMORY_FORMAT);
      src_contig = iter.tensor(1).expand_as(dst).contiguous();
    }

    // perform a same-dtype copy on contiguous tensors
    TORCH_INTERNAL_ASSERT(dst_contig.sizes().equals(src_contig.sizes()));
    TORCH_INTERNAL_ASSERT(dst_contig.scalar_type() == src_contig.scalar_type());
    dst_contig.copy_(src_contig, non_blocking);

    // if necessary, copy back into dst
    if (!dst_contig.is_same(dst)) {
      TORCH_INTERNAL_ASSERT(dst_contig.device() == dst.device());
      dst.copy_(dst_contig, non_blocking);
    }
    return;
  }

  // Copy on GPU (or between GPUs)
  if (dst_device.type() == c10::DeviceType::XPU &&
      src_device.type() == c10::DeviceType::XPU) {
    copy_device_to_device(iter, non_blocking);
    return;
  }

  // Copy between CPU and GPU
  OptionalDPCPPGuard device_guard;
  dpcppMemcpyKind kind;
  if (dst_device.type() == c10::DeviceType::XPU && src_device.is_cpu()) {
    device_guard.set_device(dst_device);
    kind = HostToDevice;
  } else if (dst_device.is_cpu() && src_device.type() == c10::DeviceType::XPU) {
    device_guard.set_device(src_device);
    kind = DeviceToHost;
  } else {
    TORCH_INTERNAL_ASSERT(false, "unsupported devices in GPU copy_()");
  }

  void* dst = iter.data_ptr(0);
  void* src = iter.data_ptr(1);
  int64_t nbytes = iter.numel() * iter.element_size(0);

  if (non_blocking) {
    // here do the dpcpp copy synchronisation.
    // we use a very simple version for the singleton sycl queue.
    // TODO: enhance the full functionality in multi-queue scenario.
    dpcppMemcpyAsync(dst, src, nbytes, kind);
  } else {
    dpcppMemcpy(dst, src, nbytes, kind);
  }
}

template <typename T>
inline void array_copy(T* dst, const T* src, size_t size) {
  for (size_t i = 0; i < size; ++i)
    dst[i] = src[i];
}

inline bool onednn_strides_check(const Tensor& src) {
  auto adims = xpu::oneDNN::get_onednn_dims(src);
  int ndims = (int)adims.size();
  auto dims = adims.data();
  auto data_type = static_cast<dnnl_data_type_t>(
      xpu::oneDNN::get_onednn_dtype(src, /*allow_undef*/ true));
  auto strides_info = xpu::oneDNN::get_onednn_strides(src);
  auto strides = strides_info.empty() ? nullptr : &strides_info[0];

  auto md = dnnl_memory_desc_t();
  md.ndims = ndims;
  array_copy(md.dims, dims, ndims);
  md.data_type = data_type;
  array_copy(md.padded_dims, dims, ndims);
  md.format_kind = dnnl_format_kind_t::dnnl_blocked;
  if (strides == nullptr || md.ndims == 0 ||
      md.format_kind != dnnl_format_kind_t::dnnl_blocked)
    return true;

  dnnl_dims_t blocks = {0};
  int perm[DNNL_MAX_NDIMS] = {0};
  for (int d = 0; d < md.ndims; ++d) {
    // no strides check needed for empty tensor
    if (md.padded_dims[d] == 0)
      return true;

    // no strides verification for runtime dims
    if (strides[d] == DNNL_RUNTIME_DIM_VAL)
      return true;

    perm[d] = d;
    blocks[d] = 1;
  }

  auto block_size = 1;
  const auto& blk = md.format_desc.blocking;
  for (int iblk = 0; iblk < blk.inner_nblks; ++iblk) {
    blocks[blk.inner_idxs[iblk]] *= blk.inner_blks[iblk];
    block_size *= blk.inner_blks[iblk];
  }

  // A custom comparator to yield linear order on perm
  auto idx_sorter = [&](const int a, const int b) -> bool {
    if (strides[a] == strides[b] && md.padded_dims[a] == md.padded_dims[b])
      return a < b;
    else if (strides[a] == strides[b])
      return md.padded_dims[a] < md.padded_dims[b];
    else
      return strides[a] < strides[b];
  };
  std::sort(perm, perm + md.ndims, idx_sorter);

  auto min_stride = block_size;
  for (int idx = 0; idx < md.ndims; ++idx) {
    const int d = perm[idx];

    // Make an exception for strides[d] == 0 as it has broadcast semantics
    // Note: owing to being sorted, these are the initial strides
    if (strides[d] == 0)
      continue;
    else if (strides[d] < min_stride)
      return false;

    // update min_stride for next iteration
    const auto padded_dim = md.padded_dims[d];
    min_stride = block_size * strides[d] * (padded_dim / blocks[d]);
  }
  return true;
}

Tensor& copy_(Tensor& self, const Tensor& src, bool non_blocking) {
  // TODO: valid check
  if (self.is_same(src)) {
    return self;
  }

  if (self.is_quantized() && src.is_quantized()) {
    auto mfmt = self.is_contiguous(at::MemoryFormat::ChannelsLast)
        ? at::MemoryFormat::ChannelsLast
        : at::MemoryFormat::Contiguous;
    self = _empty_affine_quantized(
        self.sizes(), self.options(), 1.f, static_cast<int>(0), mfmt);
    set_quantizer_(self, src.quantizer());
  }

  BUILD_TENSOR_ITER(self, src, iter);

  if (iter.numel() == 0) {
    return self;
  }

  at::Device src_device = src.device();
  at::Device dst_device = self.device();

  bool same_device =
      src_device.type() == c10::DeviceType::XPU && src_device == dst_device;
  bool has_sz_st = src.sizes().size() != 0 && src.strides().size() != 0 &&
      self.sizes().size() != 0 && self.strides().size() != 0;
  // oneDNN reorder supports maximun dimension is 6,
  // so we call copy_kernel_dpcpp when dimension larger than 6.
  // TODO: All the plain format D2D copy will call copy_kernel_dpcpp
  // once our loops kernel is ready.
  if (same_device && has_sz_st && onednn_strides_check(self) &&
      onednn_strides_check(src) &&
      xpu::oneDNN::is_supported_onednn_dtype(self) &&
      xpu::oneDNN::is_supported_onednn_dtype(src) &&
      src.dim() <= MAX_ONEDNN_SUPPORTED_DIMS) {
    xpu::oneDNN::reorder_copy(src, self);
  } else {
    impl::copy_kernel_dpcpp(iter, non_blocking);
  }
  return self;
}
} // namespace impl

namespace AtenIpexTypeXPU {
Tensor& copy_(Tensor& self, const Tensor& src, bool non_blocking) {
  return impl::copy_(self, src, non_blocking);
}

Tensor _to_copy(
    const Tensor& self,
    c10::optional<ScalarType> dtype,
    c10::optional<Layout> layout,
    c10::optional<Device> device,
    c10::optional<bool> pin_memory,
    bool non_blocking,
    c10::optional<c10::MemoryFormat> optional_memory_format) {
  return at::native::_to_copy(
      self,
      dtype,
      layout,
      device,
      pin_memory,
      non_blocking,
      optional_memory_format);
}
} // namespace AtenIpexTypeXPU

namespace AtenIpexTypeQuantizedXPU {
Tensor& copy_(Tensor& self, const Tensor& src, bool non_blocking) {
  return impl::copy_(self, src, non_blocking);
}

Tensor _to_copy(
    const Tensor& self,
    c10::optional<ScalarType> dtype,
    c10::optional<Layout> layout,
    c10::optional<Device> device,
    c10::optional<bool> pin_memory,
    bool non_blocking,
    c10::optional<c10::MemoryFormat> optional_memory_format) {
  return at::native::_to_copy(
      self,
      dtype,
      layout,
      device,
      pin_memory,
      non_blocking,
      optional_memory_format);
}
} // namespace AtenIpexTypeQuantizedXPU

namespace AtenIpexTypeSparseXPU {
Tensor& copy_(Tensor& self, const Tensor& src, bool non_blocking) {
  return at::impl::copy_(self, src, non_blocking);
}

Tensor _to_copy(
    const Tensor& self,
    c10::optional<ScalarType> dtype,
    c10::optional<Layout> layout,
    c10::optional<Device> device,
    c10::optional<bool> pin_memory,
    bool non_blocking,
    c10::optional<c10::MemoryFormat> optional_memory_format) {
  return at::native::_to_copy(
      self,
      dtype,
      layout,
      device,
      pin_memory,
      non_blocking,
      optional_memory_format);
}
} // namespace AtenIpexTypeSparseXPU
} // namespace at