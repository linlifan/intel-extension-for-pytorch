// Autogenerated file by gen-gpu-ops.py. Do not edit directly!

#include <ATen/Tensor.h>

namespace at {

class AtenIpexTypeDefault {
 public:
  static at::Tensor & abs_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & acos_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor add(const at::Tensor & self, const at::Tensor & other, at::Scalar alpha);
  static at::Tensor & add_(at::Tensor & self, const at::Tensor & other, at::Scalar alpha);
  static at::Tensor & add_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other, at::Scalar alpha);
  static at::Tensor add(const at::Tensor & self, at::Scalar other, at::Scalar alpha);
  static at::Tensor & add_(at::Tensor & self, at::Scalar other, at::Scalar alpha);
  static at::Tensor & arange_out(at::Tensor & out, at::Scalar start, at::Scalar end, at::Scalar step);
  static at::Tensor as_strided(const at::Tensor & self, at::IntArrayRef size, at::IntArrayRef stride, c10::optional<int64_t> storage_offset);
  static at::Tensor & asin_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & atan_(at::Tensor & self);
  static at::Tensor & atan_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor bitwise_not(const at::Tensor & self);
  static at::Tensor & bitwise_not_(at::Tensor & self);
  static at::Tensor & bitwise_not_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor logical_not(const at::Tensor & self);
  static at::Tensor & logical_not_(at::Tensor & self);
  static at::Tensor & logical_not_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & ceil_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & clamp_(at::Tensor & self, c10::optional<at::Scalar> min, c10::optional<at::Scalar> max);
  static at::Tensor & clamp_out(at::Tensor & out, const at::Tensor & self, c10::optional<at::Scalar> min, c10::optional<at::Scalar> max);
  static at::Tensor & clamp_max_(at::Tensor & self, at::Scalar max);
  static at::Tensor & clamp_max_out(at::Tensor & out, const at::Tensor & self, at::Scalar max);
  static at::Tensor & clamp_min_(at::Tensor & self, at::Scalar min);
  static at::Tensor & clamp_min_out(at::Tensor & out, const at::Tensor & self, at::Scalar min);
  static at::Tensor convolution_overrideable(const at::Tensor & input, const at::Tensor & weight, const at::Tensor & bias, at::IntArrayRef stride, at::IntArrayRef padding, at::IntArrayRef dilation, bool transposed, at::IntArrayRef output_padding, int64_t groups);
  static std::tuple<at::Tensor,at::Tensor,at::Tensor> convolution_backward_overrideable(const at::Tensor & grad_output, const at::Tensor & input, const at::Tensor & weight, at::IntArrayRef stride, at::IntArrayRef padding, at::IntArrayRef dilation, bool transposed, at::IntArrayRef output_padding, int64_t groups, std::array<bool,3> output_mask);
  static at::Tensor & copy_(at::Tensor & self, const at::Tensor & src, bool non_blocking);
  static at::Tensor & cos_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & cosh_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor div(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor & div_(at::Tensor & self, const at::Tensor & other);
  static at::Tensor & div_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other);
  static std::tuple<at::Tensor,at::Tensor,at::Tensor,at::Tensor> _embedding_bag(const at::Tensor & weight, const at::Tensor & indices, const at::Tensor & offsets, bool scale_grad_by_freq, int64_t mode, bool sparse, const at::Tensor & per_sample_weights);
  static at::Tensor _embedding_bag_dense_backward(const at::Tensor & grad, const at::Tensor & indices, const at::Tensor & offsets, const at::Tensor & offset2bag, const at::Tensor & bag_size, const at::Tensor & maximum_indices, int64_t num_weights, bool scale_grad_by_freq, int64_t mode, const at::Tensor & per_sample_weights);
  static at::Tensor empty(at::IntArrayRef size, const at::TensorOptions & options, c10::optional<at::MemoryFormat> memory_format);
  static at::Tensor & resize_(at::Tensor & self, at::IntArrayRef size, c10::optional<at::MemoryFormat> memory_format);
  static at::Tensor empty_strided(at::IntArrayRef size, at::IntArrayRef stride, const at::TensorOptions & options);
  static at::Tensor & erf_(at::Tensor & self);
  static at::Tensor & erf_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & erfc_(at::Tensor & self);
  static at::Tensor & erfc_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & exp_(at::Tensor & self);
  static at::Tensor & exp_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & expm1_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & eye_out(at::Tensor & out, int64_t n);
  static at::Tensor & eye_out(at::Tensor & out, int64_t n, int64_t m);
  static at::Tensor & fill_(at::Tensor & self, at::Scalar value);
  static at::Tensor & fill_(at::Tensor & self, const at::Tensor & value);
  static at::Tensor & floor_out(at::Tensor & out, const at::Tensor & self);
  static std::tuple<at::Tensor &,at::Tensor &> kthvalue_out(at::Tensor & values, at::Tensor & indices, const at::Tensor & self, int64_t k, int64_t dim, bool keepdim);
  static at::Tensor & linspace_out(at::Tensor & out, at::Scalar start, at::Scalar end, int64_t steps);
  static at::Tensor & log_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & log10_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & log1p_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & log2_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & logspace_out(at::Tensor & out, at::Scalar start, at::Scalar end, int64_t steps, double base);
  static at::Tensor _log_softmax(const at::Tensor & self, int64_t dim, bool half_to_float);
  static at::Tensor _log_softmax_backward_data(const at::Tensor & grad_output, const at::Tensor & output, int64_t dim, const at::Tensor & self);
  static std::tuple<at::Tensor,at::Tensor> max(const at::Tensor & self, int64_t dim, bool keepdim);
  static std::tuple<at::Tensor &,at::Tensor &> max_out(at::Tensor & max, at::Tensor & max_values, const at::Tensor & self, int64_t dim, bool keepdim);
  static std::tuple<at::Tensor,at::Tensor> min(const at::Tensor & self, int64_t dim, bool keepdim);
  static std::tuple<at::Tensor &,at::Tensor &> min_out(at::Tensor & min, at::Tensor & min_indices, const at::Tensor & self, int64_t dim, bool keepdim);
  static at::Tensor mm(const at::Tensor & self, const at::Tensor & mat2);
  static at::Tensor & mm_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & mat2);
  static at::Tensor mul(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor & mul_(at::Tensor & self, const at::Tensor & other);
  static at::Tensor & mul_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other);
  static at::Tensor mul(const at::Tensor & self, at::Scalar other);
  static at::Tensor & mul_(at::Tensor & self, at::Scalar other);
  static at::Tensor narrow_copy(const at::Tensor & self, int64_t dim, int64_t start, int64_t length);
  static std::tuple<at::Tensor,at::Tensor,at::Tensor> native_batch_norm(const at::Tensor & input, const at::Tensor & weight, const at::Tensor & bias, const at::Tensor & running_mean, const at::Tensor & running_var, bool training, double momentum, double eps);
  static std::tuple<at::Tensor,at::Tensor,at::Tensor> native_batch_norm_backward(const at::Tensor & grad_out, const at::Tensor & input, const at::Tensor & weight, const at::Tensor & running_mean, const at::Tensor & running_var, const at::Tensor & save_mean, const at::Tensor & save_invstd, bool train, double eps, std::array<bool,3> output_mask);
  static at::Tensor & range_out(at::Tensor & out, at::Scalar start, at::Scalar end, at::Scalar step);
  static at::Tensor & neg_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & round_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & relu_(at::Tensor & self);
  static at::Tensor & rsqrt_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor sigmoid(const at::Tensor & self);
  static at::Tensor & sigmoid_(at::Tensor & self);
  static at::Tensor & sigmoid_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & sin_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & sinh_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor _softmax(const at::Tensor & self, int64_t dim, bool half_to_float);
  static at::Tensor _softmax_backward_data(const at::Tensor & grad_output, const at::Tensor & output, int64_t dim, const at::Tensor & self);
  static at::Tensor sum(const at::Tensor & self, c10::optional<at::ScalarType> dtype);
  static at::Tensor sum(const at::Tensor & self, at::IntArrayRef dim, bool keepdim, c10::optional<at::ScalarType> dtype);
  static at::Tensor & sum_out(at::Tensor & out, const at::Tensor & self, at::IntArrayRef dim, bool keepdim, c10::optional<at::ScalarType> dtype);
  static at::Tensor & sqrt_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & tan_(at::Tensor & self);
  static at::Tensor & tan_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & tanh_(at::Tensor & self);
  static at::Tensor & tanh_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor threshold(const at::Tensor & self, at::Scalar threshold, at::Scalar value);
  static at::Tensor & threshold_(at::Tensor & self, at::Scalar threshold, at::Scalar value);
  static at::Tensor & threshold_out(at::Tensor & out, const at::Tensor & self, at::Scalar threshold, at::Scalar value);
  static at::Tensor roll(const at::Tensor & self, at::IntArrayRef shifts, at::IntArrayRef dims);
  static at::Tensor & trunc_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor norm(const at::Tensor & self, c10::optional<at::Scalar> p, at::ScalarType dtype);
  static at::Tensor norm(const at::Tensor & self, at::Scalar p);
  static at::Tensor norm(const at::Tensor & self, c10::optional<at::Scalar> p, at::IntArrayRef dim, bool keepdim, at::ScalarType dtype);
  static at::Tensor norm(const at::Tensor & self, c10::optional<at::Scalar> p, at::IntArrayRef dim, bool keepdim);
  static at::Tensor & norm_out(at::Tensor & out, const at::Tensor & self, c10::optional<at::Scalar> p, at::IntArrayRef dim, bool keepdim, at::ScalarType dtype);
  static at::Tensor & norm_out(at::Tensor & out, const at::Tensor & self, c10::optional<at::Scalar> p, at::IntArrayRef dim, bool keepdim);
  static at::Tensor clone(const at::Tensor & self, c10::optional<at::MemoryFormat> memory_format);
  static at::Tensor & resize_as_(at::Tensor & self, const at::Tensor & the_template, c10::optional<at::MemoryFormat> memory_format);
  static at::Tensor & pow_out(at::Tensor & out, const at::Tensor & self, at::Scalar exponent);
  static at::Tensor pow(const at::Tensor & self, at::Scalar exponent);
  static at::Tensor & zero_(at::Tensor & self);
  static at::Tensor & sub_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other, at::Scalar alpha);
  static at::Tensor sub(const at::Tensor & self, const at::Tensor & other, at::Scalar alpha);
  static at::Tensor & sub_(at::Tensor & self, const at::Tensor & other, at::Scalar alpha);
  static at::Tensor sub(const at::Tensor & self, at::Scalar other, at::Scalar alpha);
  static at::Tensor & sub_(at::Tensor & self, at::Scalar other, at::Scalar alpha);
  static at::Tensor rsub(const at::Tensor & self, const at::Tensor & other, at::Scalar alpha);
  static at::Tensor rsub(const at::Tensor & self, at::Scalar other, at::Scalar alpha);
  static at::Tensor addmm(const at::Tensor & self, const at::Tensor & mat1, const at::Tensor & mat2, at::Scalar beta, at::Scalar alpha);
  static at::Scalar _local_scalar_dense(const at::Tensor & self);
  static at::Tensor & set_(at::Tensor & self, at::Storage source);
  static at::Tensor & set_(at::Tensor & self, at::Storage source, int64_t storage_offset, at::IntArrayRef size, at::IntArrayRef stride);
  static at::Tensor & set_(at::Tensor & self, const at::Tensor & source);
  static at::Tensor & set_(at::Tensor & self);
  static at::Tensor view(const at::Tensor & self, at::IntArrayRef size);
  static at::Tensor __and__(const at::Tensor & self, at::Scalar other);
  static at::Tensor __and__(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor & __iand__(at::Tensor & self, at::Scalar other);
  static at::Tensor & __iand__(at::Tensor & self, const at::Tensor & other);
  static at::Tensor __or__(const at::Tensor & self, at::Scalar other);
  static at::Tensor __or__(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor & __ior__(at::Tensor & self, at::Scalar other);
  static at::Tensor & __ior__(at::Tensor & self, const at::Tensor & other);
  static at::Tensor & tril_(at::Tensor & self, int64_t diagonal);
  static at::Tensor & triu_(at::Tensor & self, int64_t diagonal);
  static at::Tensor & pow_(at::Tensor & self, at::Scalar exponent);
  static at::Tensor & pow_(at::Tensor & self, const at::Tensor & exponent);
  static at::Tensor & addcdiv_(at::Tensor & self, const at::Tensor & tensor1, const at::Tensor & tensor2, at::Scalar value);
  static at::Tensor & triu_out(at::Tensor & out, const at::Tensor & self, int64_t diagonal);
  static at::Tensor & tril_out(at::Tensor & out, const at::Tensor & self, int64_t diagonal);
  static at::Tensor tril_indices(int64_t row, int64_t col, int64_t offset, const at::TensorOptions & options);
  static at::Tensor triu_indices(int64_t row, int64_t col, int64_t offset, const at::TensorOptions & options);
  static at::Tensor & ne_out(at::Tensor & out, const at::Tensor & self, at::Scalar other);
  static at::Tensor ne(const at::Tensor & self, at::Scalar other);
  static at::Tensor & ne_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other);
  static at::Tensor ne(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor & eq_out(at::Tensor & out, const at::Tensor & self, at::Scalar other);
  static at::Tensor eq(const at::Tensor & self, at::Scalar other);
  static at::Tensor & eq_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other);
  static at::Tensor eq(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor & ge_out(at::Tensor & out, const at::Tensor & self, at::Scalar other);
  static at::Tensor ge(const at::Tensor & self, at::Scalar other);
  static at::Tensor & ge_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other);
  static at::Tensor ge(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor & le_out(at::Tensor & out, const at::Tensor & self, at::Scalar other);
  static at::Tensor le(const at::Tensor & self, at::Scalar other);
  static at::Tensor & le_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other);
  static at::Tensor le(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor & gt_out(at::Tensor & out, const at::Tensor & self, at::Scalar other);
  static at::Tensor gt(const at::Tensor & self, at::Scalar other);
  static at::Tensor & gt_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other);
  static at::Tensor gt(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor & lt_out(at::Tensor & out, const at::Tensor & self, at::Scalar other);
  static at::Tensor lt(const at::Tensor & self, at::Scalar other);
  static at::Tensor & lt_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other);
  static at::Tensor lt(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor & index_select_out(at::Tensor & out, const at::Tensor & self, int64_t dim, const at::Tensor & index);
  static at::Tensor index_select(const at::Tensor & self, int64_t dim, const at::Tensor & index);
  static at::Tensor & addcmul_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & tensor1, const at::Tensor & tensor2, at::Scalar value);
  static at::Tensor addcmul(const at::Tensor & self, const at::Tensor & tensor1, const at::Tensor & tensor2, at::Scalar value);
  static at::Tensor & addcmul_(at::Tensor & self, const at::Tensor & tensor1, const at::Tensor & tensor2, at::Scalar value);
  static at::Tensor & addcdiv_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & tensor1, const at::Tensor & tensor2, at::Scalar value);
  static at::Tensor addcdiv(const at::Tensor & self, const at::Tensor & tensor1, const at::Tensor & tensor2, at::Scalar value);
  static at::Tensor & min_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other);
  static at::Tensor min(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor min(const at::Tensor & self);
  static at::Tensor & max_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other);
  static at::Tensor max(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor max(const at::Tensor & self);
  static at::Tensor median(const at::Tensor & self);
  static std::tuple<at::Tensor &,at::Tensor &> sort_out(at::Tensor & values, at::Tensor & indices, const at::Tensor & self, int64_t dim, bool descending);
  static std::tuple<at::Tensor,at::Tensor> sort(const at::Tensor & self, int64_t dim, bool descending);
  static std::tuple<at::Tensor &,at::Tensor &> topk_out(at::Tensor & values, at::Tensor & indices, const at::Tensor & self, int64_t k, int64_t dim, bool largest, bool sorted);
  static std::tuple<at::Tensor,at::Tensor> topk(const at::Tensor & self, int64_t k, int64_t dim, bool largest, bool sorted);
  static at::Tensor & pow_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & exponent);
  static at::Tensor pow(const at::Tensor & self, const at::Tensor & exponent);
  static at::Tensor & pow_out(at::Tensor & out, at::Scalar self, const at::Tensor & exponent);
  static at::Tensor pow(at::Scalar self, const at::Tensor & exponent);
  static std::tuple<at::Tensor,at::Tensor> _max(const at::Tensor & self, int64_t dim, bool keepdim);
  static std::tuple<at::Tensor &,at::Tensor &> _max_out(at::Tensor & max, at::Tensor & max_indices, const at::Tensor & self, int64_t dim, bool keepdim);
  static std::tuple<at::Tensor,at::Tensor> _min(const at::Tensor & self, int64_t dim, bool keepdim);
  static std::tuple<at::Tensor &,at::Tensor &> _min_out(at::Tensor & min, at::Tensor & min_indices, const at::Tensor & self, int64_t dim, bool keepdim);
  static at::Tensor & binary_cross_entropy_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & target, const at::Tensor & weight, int64_t reduction);
  static at::Tensor binary_cross_entropy(const at::Tensor & self, const at::Tensor & target, const at::Tensor & weight, int64_t reduction);
  static at::Tensor & binary_cross_entropy_backward_out(at::Tensor & grad_input, const at::Tensor & grad_output, const at::Tensor & self, const at::Tensor & target, const at::Tensor & weight, int64_t reduction);
  static at::Tensor binary_cross_entropy_backward(const at::Tensor & grad_output, const at::Tensor & self, const at::Tensor & target, const at::Tensor & weight, int64_t reduction);
  static at::Tensor & mse_loss_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & target, int64_t reduction);
  static at::Tensor mse_loss(const at::Tensor & self, const at::Tensor & target, int64_t reduction);
  static std::tuple<at::Tensor &,at::Tensor &> nll_loss_forward_out(at::Tensor & output, at::Tensor & total_weight, const at::Tensor & self, const at::Tensor & target, const at::Tensor & weight, int64_t reduction, int64_t ignore_index);
  static std::tuple<at::Tensor,at::Tensor> nll_loss_forward(const at::Tensor & self, const at::Tensor & target, const at::Tensor & weight, int64_t reduction, int64_t ignore_index);
  static at::Tensor & nll_loss_backward_out(at::Tensor & grad_input, const at::Tensor & grad_output, const at::Tensor & self, const at::Tensor & target, const at::Tensor & weight, int64_t reduction, int64_t ignore_index, const at::Tensor & total_weight);
  static at::Tensor nll_loss_backward(const at::Tensor & grad_output, const at::Tensor & self, const at::Tensor & target, const at::Tensor & weight, int64_t reduction, int64_t ignore_index, const at::Tensor & total_weight);
  static at::Tensor & softplus_out(at::Tensor & out, const at::Tensor & self, at::Scalar beta, at::Scalar threshold);
  static at::Tensor softplus(const at::Tensor & self, at::Scalar beta, at::Scalar threshold);
  static at::Tensor & softplus_backward_out(at::Tensor & grad_input, const at::Tensor & grad_output, const at::Tensor & self, at::Scalar beta, at::Scalar threshold, const at::Tensor & output);
  static at::Tensor softplus_backward(const at::Tensor & grad_output, const at::Tensor & self, at::Scalar beta, at::Scalar threshold, const at::Tensor & output);
  static at::Tensor & adaptive_avg_pool2d_out(at::Tensor & out, const at::Tensor & self, at::IntArrayRef output_size);
  static at::Tensor adaptive_avg_pool2d(const at::Tensor & self, at::IntArrayRef output_size);
  static at::Tensor _adaptive_avg_pool2d(const at::Tensor & self, at::IntArrayRef output_size);
  static at::Tensor _adaptive_avg_pool2d_backward(const at::Tensor & grad_output, const at::Tensor & self);
  static at::Tensor & avg_pool2d_out(at::Tensor & out, const at::Tensor & self, at::IntArrayRef kernel_size, at::IntArrayRef stride, at::IntArrayRef padding, bool ceil_mode, bool count_include_pad, c10::optional<int64_t> divisor_override);
  static at::Tensor avg_pool2d(const at::Tensor & self, at::IntArrayRef kernel_size, at::IntArrayRef stride, at::IntArrayRef padding, bool ceil_mode, bool count_include_pad, c10::optional<int64_t> divisor_override);
  static at::Tensor & avg_pool2d_backward_out(at::Tensor & grad_input, const at::Tensor & grad_output, const at::Tensor & self, at::IntArrayRef kernel_size, at::IntArrayRef stride, at::IntArrayRef padding, bool ceil_mode, bool count_include_pad, c10::optional<int64_t> divisor_override);
  static at::Tensor avg_pool2d_backward(const at::Tensor & grad_output, const at::Tensor & self, at::IntArrayRef kernel_size, at::IntArrayRef stride, at::IntArrayRef padding, bool ceil_mode, bool count_include_pad, c10::optional<int64_t> divisor_override);
  static std::tuple<at::Tensor &,at::Tensor &> max_pool2d_with_indices_out(at::Tensor & out, at::Tensor & indices, const at::Tensor & self, at::IntArrayRef kernel_size, at::IntArrayRef stride, at::IntArrayRef padding, at::IntArrayRef dilation, bool ceil_mode);
  static std::tuple<at::Tensor,at::Tensor> max_pool2d_with_indices(const at::Tensor & self, at::IntArrayRef kernel_size, at::IntArrayRef stride, at::IntArrayRef padding, at::IntArrayRef dilation, bool ceil_mode);
  static at::Tensor & max_pool2d_with_indices_backward_out(at::Tensor & grad_input, const at::Tensor & grad_output, const at::Tensor & self, at::IntArrayRef kernel_size, at::IntArrayRef stride, at::IntArrayRef padding, at::IntArrayRef dilation, bool ceil_mode, const at::Tensor & indices);
  static at::Tensor max_pool2d_with_indices_backward(const at::Tensor & grad_output, const at::Tensor & self, at::IntArrayRef kernel_size, at::IntArrayRef stride, at::IntArrayRef padding, at::IntArrayRef dilation, bool ceil_mode, const at::Tensor & indices);
  static at::Tensor & upsample_nearest2d_out(at::Tensor & out, const at::Tensor & self, at::IntArrayRef output_size);
  static at::Tensor upsample_nearest2d(const at::Tensor & self, at::IntArrayRef output_size);
  static at::Tensor & upsample_nearest2d_backward_out(at::Tensor & grad_input, const at::Tensor & grad_output, at::IntArrayRef output_size, at::IntArrayRef input_size);
  static at::Tensor upsample_nearest2d_backward(const at::Tensor & grad_output, at::IntArrayRef output_size, at::IntArrayRef input_size);
  static at::Tensor & sigmoid_backward_out(at::Tensor & grad_input, const at::Tensor & grad_output, const at::Tensor & output);
  static at::Tensor sigmoid_backward(const at::Tensor & grad_output, const at::Tensor & output);
  static at::Tensor & tanh_backward_out(at::Tensor & grad_input, const at::Tensor & grad_output, const at::Tensor & output);
  static at::Tensor tanh_backward(const at::Tensor & grad_output, const at::Tensor & output);

};

void RegisterAtenTypeFunctions();

}  // namespace at

