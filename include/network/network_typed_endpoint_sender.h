#ifndef STONKS_NETWORK_NETWORK_TYPED_ENDPOINT_SENDER_H_
#define STONKS_NETWORK_NETWORK_TYPED_ENDPOINT_SENDER_H_

#include <memory>

#include "network_i_rest_request_sender.h"
#include "network_typed_endpoint.h"
#include "network_types.h"
#include "not_null.hpp"

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
      cpp::not_null<std::shared_ptr<IRestRequestSender>> request_sender);

  /**
   * @brief Validates request prior forwarding it to the sender and response
   * after receiving it from sender.
   */
  [[nodiscard]] auto SendRequestAndGetResponse(RestRequest request) const
      -> RestResponse override;

 private:
  void ValidateRequest(const RestRequest &request) const;
  void ValidateRequestParamTypes(const Params &params) const;
  void ValidateRequestBodyType(const Body &body) const;
  void ValidateResponseTypes(const RestResponse &response) const;

  EndpointTypes endpoint_types_{};
  cpp::not_null<std::shared_ptr<IRestRequestSender>> request_sender_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_TYPED_ENDPOINT_SENDER_H_