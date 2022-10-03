#include "network_rest_client.h"

#include <string>
#include <utility>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "cpp_typed_struct.h"
#include "di_factory.h"
#include "network_i_rest_request_sender.h"
#include "network_response_exception_handler.h"
#include "network_typed_endpoint_sender.h"
#include "network_types.h"

namespace stonks::network {
RestClient::RestClient(Uri base_uri,
                       di::Factory<IRestRequestSender> request_sender_factory)
    : base_uri_{std::move(base_uri)},
      request_sender_factory_{std::move(request_sender_factory)} {}

auto RestClient::Call(TypedEndpoint endpoint) const
    -> rest_client::RequestBuilder {
  endpoint.endpoint.uri.value = base_uri_.value + endpoint.endpoint.uri.value;

  auto decorated_sender = cpp::MakeNnUp<ResponseExceptionHandler>(
      cpp::MakeNnUp<TypedEndpointSender>(std::move(endpoint.expected_types),
                                         request_sender_factory_.Create()));

  return cpp::CallExposedPrivateConstructorOf<rest_client::RequestBuilder,
                                              RestClient>{}(
      std::move(endpoint.endpoint), std::move(decorated_sender));
}
}  // namespace stonks::network