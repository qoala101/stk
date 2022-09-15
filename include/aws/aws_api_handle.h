#ifndef STONKS_AWS_AWS_API_HANDLE_H_
#define STONKS_AWS_AWS_API_HANDLE_H_

#include "cpp_not_null.h"
#include "log_i_logger.h"

namespace stonks::aws {
namespace detail {
class ApiHandleImpl;
}  // namespace detail

/**
 * @brief AWS API lifetime handle.
 * All AWS clients should keep an instance for API to work.
 */
class ApiHandle {
 public:
  explicit ApiHandle(cpp::NnSp<log::ILogger> logger);

 private:
  cpp::NnSp<detail::ApiHandleImpl> impl_;
};
}  // namespace stonks::aws

#endif  // STONKS_AWS_AWS_API_HANDLE_H_
