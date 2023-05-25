/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_AWS_API_HANDLE_H_
#define VH_AWS_API_HANDLE_H_

#include "cpp_not_null.h"
#include "log_i_logger.h"

namespace vh::aws {
namespace detail {
class ApiHandleImpl;
}  // namespace detail

/**
 * @brief AWS API lifetime handle.
 * All AWS clients should keep an instance for API to work.
 */
class ApiHandle {
 public:
  explicit ApiHandle(cpp::NnUp<log::ILogger> logger);

 private:
  cpp::NnSp<detail::ApiHandleImpl> impl_;
};
}  // namespace vh::aws

#endif  // VH_AWS_API_HANDLE_H_
