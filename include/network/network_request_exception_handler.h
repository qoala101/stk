#ifndef STONKS_NETWORK_NETWORK_REQUEST_EXCEPTION_HANDLER_H_
#define STONKS_NETWORK_NETWORK_REQUEST_EXCEPTION_HANDLER_H_

#include <type_traits>

#include "network_types.h"

namespace stonks::network {
/**
 * @brief Decorator of request handler which transforms the exception thrown by
 * the handler to the appropriate response.
 */
class RequestExceptionHandler {
 public:
  explicit RequestExceptionHandler(RestRequestHandler handler);

  /**
   * @brief Catches exception thrown by the handler and transforms it
   * to the response.
   */
  auto operator()(RestRequest request) const -> RestResponse;

 private:
  RestRequestHandler handler_{};
};

static_assert(
    std::is_convertible_v<RequestExceptionHandler, RestRequestHandler>);
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REQUEST_EXCEPTION_HANDLER_H_
