#ifndef STONKS_AWS_AWS_API_HANDLE_H_
#define STONKS_AWS_AWS_API_HANDLE_H_

#include "cpp_not_null.h"

namespace stonks::aws {
/**
 * @brief AWS API lifetime handle.
 * All AWS clients should keep an instance for API to work.
 */
class ApiHandle {
 public:
  ApiHandle();

 private:
  class ApiHandleImpl;

  cpp::NnSp<ApiHandleImpl> impl_;
};
}  // namespace stonks::aws

#endif  // STONKS_AWS_AWS_API_HANDLE_H_
