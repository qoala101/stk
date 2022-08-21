#include "network_rest_client.h"

#include <utility>

#include "ccutils_expose_private_constructors.h"
#include "ccutils_not_null.h"
#include "network_i_rest_request_sender.h"
#include "network_response_exception_handler.h"
#include "network_typed_endpoint_sender.h"
#include "network_types.h"

namespace stonks::network {
RestClient::RestClient(std::string base_uri,
                       ccutils::NnSp<IRestRequestSender> request_sender)
    : base_uri_{std::move(base_uri)},
      request_sender_{std::move(request_sender)} {}

auto RestClient::Call(TypedEndpoint endpoint) const
    -> rest_client::RequestBuilder {
  endpoint.endpoint.uri = base_uri_ + endpoint.endpoint.uri;

  auto typed_sender = ccutils::MakeNnUp<ResponseExceptionHandler>(
      ccutils::MakeNnUp<TypedEndpointSender>(std::move(endpoint.expected_types),
                                             request_sender_));

  return ccutils::CallExposedPrivateConstructorOf<rest_client::RequestBuilder,
                                                  RestClient>{}(
      std::move(endpoint.endpoint), std::move(typed_sender));
}
}  // namespace stonks::network