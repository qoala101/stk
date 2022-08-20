#ifndef STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_ENDPOINT_HANDLER_H_
#define STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_ENDPOINT_HANDLER_H_

#include <memory>

#include "network_endpoint_types_validator_template.h"
#include "network_i_rest_request_handler.h"
#include "network_typed_endpoint.h"
#include "network_types.h"
#include "not_null.hpp"

namespace stonks::network {
/**
 * @brief Decorator of request handler which adds validation of request
 * and response types.
 */
class TypedEndpointHandler : public IRestRequestHandler {
 public:
  /**
   * @param endpoint_types Request and response would be validated against
   * those.
   */
  explicit TypedEndpointHandler(
      EndpointTypes endpoint_types,
      cpp::not_null<std::unique_ptr<IRestRequestHandler>> handler);

  /**
   * @brief Validates request prior forwarding it to the handler and response
   * after receiving it from handler.
   */
  [[nodiscard]] auto HandleRequestAndGiveResponse(RestRequest request) const
      -> RestResponse override;

 private:
  cpp::not_null<std::unique_ptr<EndpointTypesValidatorTemplate>> type_checker_;
  cpp::not_null<std::unique_ptr<IRestRequestHandler>> handler_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_ENDPOINT_HANDLER_H_
