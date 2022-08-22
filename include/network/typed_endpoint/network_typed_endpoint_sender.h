#ifndef STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_ENDPOINT_SENDER_H_
#define STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_ENDPOINT_SENDER_H_

#include "ccutils_not_null.h"
#include "network_endpoint_types_validator_template.h"
#include "network_i_rest_request_sender.h"
#include "network_typed_endpoint.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Decorator of request sender which adds
 * validation of request and response types.
 */
class TypedEndpointSender : public IRestRequestSender {
 public:
  /**
   * @param endpoint_types Request and response would be validated against
   * those.
   */
  explicit TypedEndpointSender(
      EndpointTypes endpoint_types,
      ccutils::NnSp<IRestRequestSender> request_sender);

  /**
   * @brief Validates request prior forwarding it to the sender and response
   * after receiving it from sender.
   */
  [[nodiscard]] auto SendRequestAndGetResponse(RestRequest request) const
      -> RestResponse override;

 private:
  ccutils::NnUp<EndpointTypesValidatorTemplate> type_checker_;
  ccutils::NnSp<IRestRequestSender> request_sender_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_ENDPOINT_SENDER_H_