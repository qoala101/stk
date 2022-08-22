#ifndef STONKS_NETWORK_NETWORK_REQUEST_EXCEPTION_HANDLER_H_
#define STONKS_NETWORK_NETWORK_REQUEST_EXCEPTION_HANDLER_H_

#include "ccutils_not_null.h"
#include "network_i_rest_request_handler.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Decorator of request handler which transforms the exception thrown by
 * the handler to the appropriate response.
 */
class RequestExceptionHandler : public IRestRequestHandler {
 public:
  explicit RequestExceptionHandler(ccutils::NnUp<IRestRequestHandler> handler);

  /**
   * @brief Catches exception thrown by the handler and transforms it
   * to the response.
   */
  [[nodiscard]] auto HandleRequestAndGiveResponse(RestRequest request) const
      -> RestResponse override;

 private:
  ccutils::NnUp<IRestRequestHandler> handler_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REQUEST_EXCEPTION_HANDLER_H_