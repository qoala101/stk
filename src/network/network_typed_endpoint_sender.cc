#include "network_typed_endpoint_sender.h"

#include <utility>

#include "network_typed_endpoint.h"

namespace stonks::network {
TypedEndpointSender::TypedEndpointSender(
    EndpointTypes endpoint_types,
    cpp::not_null<std::shared_ptr<IRestRequestSender>> request_sender)
    : endpoint_types_{std::move(endpoint_types)},
      request_sender_{std::move(request_sender)} {}

auto TypedEndpointSender::SendRequestAndGetResponse(RestRequest request) const
    -> RestResponse {
  return request_sender_->SendRequestAndGetResponse(std::move(request));
}
}  // namespace stonks::network