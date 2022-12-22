#include "networkx_client.h"

namespace stonks::networkx::detail {
ClientBase::ClientBase(
    network::Uri &base_uri,
    di::Factory<network::IRestRequestSender> &request_sender_factory)
    : rest_client_{std::move(base_uri), std::move(request_sender_factory)} {}

auto ClientBase::GetRestClient() const -> const network::RestClient & {
  return rest_client_;
}
}  // namespace stonks::networkx::detail