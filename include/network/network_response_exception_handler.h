#ifndef STONKS_NETWORK_NETWORK_RESPONSE_EXCEPTION_HANDLER_H_
#define STONKS_NETWORK_NETWORK_RESPONSE_EXCEPTION_HANDLER_H_

#include "cpp_not_null.h"
#include "network_i_rest_request_sender.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Decorator of request sender which identifies whether response
 * is an exception and throws it to the client.
 */
class ResponseExceptionHandler : public IRestRequestSender {
 public:
  explicit ResponseExceptionHandler(
      cpp::NnUp<IRestRequestSender> request_sender);

  /**
   * @brief Checks whether response is an exception, parses, and throws it.
   */
  [[nodiscard]] auto SendRequestAndGetResponse(RestRequest request) const
      -> RestResponse override;

 private:
  cpp::NnUp<IRestRequestSender> request_sender_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_RESPONSE_EXCEPTION_HANDLER_H_
