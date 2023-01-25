#include "network_rest_client.h"

#include <string>
#include <utility>

#include "cpp_meta_private_to.h"
#include "cpp_not_null.h"
#include "cpp_typed_struct.h"
#include "network_i_rest_request_sender.h"
#include "network_response_exception_handler.h"
#include "network_typed_endpoint_sender.h"
#include "network_types.h"

namespace stonks::network {
RestClient::RestClient(Uri base_uri,
                       cpp::NnUp<IRestRequestSender> request_sender)
    : base_uri_{std::move(base_uri)},
      request_sender_{std::move(request_sender)} {}

auto RestClient::Call(TypedEndpoint endpoint) const
    -> rest_client::RequestBuilder {
  *endpoint.endpoint.uri = *base_uri_ + *endpoint.endpoint.uri;

  auto decorated_sender = cpp::MakeNnUp<ResponseExceptionHandler>(
      cpp::MakeNnUp<TypedEndpointSender>(std::move(endpoint.expected_types),
                                         request_sender_));

  return {{}, std::move(endpoint.endpoint), std::move(decorated_sender)};
}
}  // namespace stonks::network