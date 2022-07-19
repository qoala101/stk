#include "network_rest_client.h"

#include <tuple>
#include <utility>

namespace stonks::network {
RestClient::RestClient(
    std::string_view base_uri,
    cpp::not_null<std::unique_ptr<IRestRequestSender>> request_sender)
    : base_uri_{base_uri}, request_sender_{std::move(request_sender)} {}

auto RestClient::CallAndGet(const Endpoint &endpoint,
                            const RestRequestData &data) const -> Result {
  const auto full_endpoint = [this, &endpoint]() {
    auto full_endpoint = endpoint;
    full_endpoint.uri = base_uri_ + full_endpoint.uri;
    return full_endpoint;
  }();

  return request_sender_->SendRequestAndGetResponse(full_endpoint, data).second;
}

void RestClient::Call(const Endpoint &endpoint,
                      const RestRequestData &data) const {
  std::ignore = CallAndGet(endpoint, data);
}
}  // namespace stonks::network