#ifndef STONKS_NETWORK_NETWORK_REQUEST_EXCEPTION_HANDLER_H_
#define STONKS_NETWORK_NETWORK_REQUEST_EXCEPTION_HANDLER_H_

#include <cppcoro/task.hpp>

#include "cpp_not_null.h"
#include "network_i_rest_request_handler.h"
#include "network_types.h"

namespace vh::network {
/**
 * @brief Decorator of request handler which transforms the exception thrown by
 * the handler to the appropriate response.
 */
class RequestExceptionHandler : public IRestRequestHandler {
 public:
  explicit RequestExceptionHandler(cpp::NnUp<IRestRequestHandler> handler);

  /**
   * @brief Catches exception thrown by the handler and transforms it
   * to the response.
   */
  auto HandleRequestAndGiveResponse [[nodiscard]] (RestRequest request)
  -> cppcoro::task<RestResponse> override;

 private:
  cpp::NnUp<IRestRequestHandler> handler_;
};
}  // namespace vh::network

#endif  // STONKS_NETWORK_NETWORK_REQUEST_EXCEPTION_HANDLER_H_