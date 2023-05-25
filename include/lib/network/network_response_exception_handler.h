/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_RESPONSE_EXCEPTION_HANDLER_H_
#define VH_NETWORK_RESPONSE_EXCEPTION_HANDLER_H_

#include <cppcoro/task.hpp>

#include "cpp_not_null.h"
#include "network_i_rest_request_sender.h"
#include "network_types.h"

namespace vh::network {
/**
 * @brief Decorator of request sender which identifies whether response
 * is an exception and throws it to the client.
 */
class ResponseExceptionHandler : public IRestRequestSender {
 public:
  explicit ResponseExceptionHandler(
      cpp::NnSp<IRestRequestSender> request_sender);

  /**
   * @brief Checks whether response is an exception, parses, and throws it.
   */
  auto SendRequestAndGetResponse [[nodiscard]] (RestRequest request)
  -> cppcoro::task<RestResponse> override;

 private:
  cpp::NnSp<IRestRequestSender> request_sender_;
};
}  // namespace vh::network

#endif  // VH_NETWORK_RESPONSE_EXCEPTION_HANDLER_H_
