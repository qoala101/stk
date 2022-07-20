#include "network_rest_client.h"

#include <memory>
#include <utility>

#include "ccutils_expose_private_constructors.h"

namespace stonks::network {
RestClient::RestClient(
    std::string_view base_uri,
    cpp::not_null<std::shared_ptr<IRestRequestSender>> request_sender)
    : base_uri_{base_uri}, request_sender_{std::move(request_sender)} {}

auto RestClient::Call(Endpoint endpoint) const -> rest_client::RequestBuilder {
  endpoint.uri = base_uri_ + endpoint.uri;
  return ccutils::CallExposedPrivateConstructorOf<rest_client::RequestBuilder,
                                                  RestClient>{}(
      std::move(endpoint), request_sender_);
}
}  // namespace stonks::network