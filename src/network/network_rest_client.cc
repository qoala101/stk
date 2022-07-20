#include "network_rest_client.h"

#include <gsl/assert>
#include <memory>
#include <tuple>
#include <utility>

#include "expose_private_constructor_to.h"

namespace stonks::network {
RestClient::RestClient(
    std::string_view base_uri,
    cpp::not_null<std::shared_ptr<IRestRequestSender>> request_sender)
    : base_uri_{base_uri}, request_sender_{std::move(request_sender)} {}

auto RestClient::Call(Endpoint endpoint) const -> rest_client::RequestBuilder {
  endpoint.uri = base_uri_ + endpoint.uri;
  return utils::CallExposedPrivateConstructorOf<rest_client::RequestBuilder,
                                                RestClient>{}(
      std::move(endpoint), request_sender_);
}
}  // namespace stonks::network