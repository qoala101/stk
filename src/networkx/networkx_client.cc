#include "networkx_client.h"

namespace stonks::networkx::detail {
ClientBase::ClientBase(network::RestClient rest_client)
    : rest_client_{std::move(rest_client)} {}

auto ClientBase::GetRestClient() const -> const network::RestClient& {
  return rest_client_;
}
}  // namespace stonks::networkx::detail