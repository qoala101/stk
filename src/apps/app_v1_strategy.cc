#include <stdio.h>

#include "ccutils_not_null.h"
#include <optional>

#include "client_proxy.h"
#include "server_v1_strategy.h"
#include "v1_strategy.h"

auto main(int /*unused*/, const char* /*unused*/[]) -> int {
  auto proxy = stonks::client::Proxy{};
  constexpr auto endpoint = stonks::server::V1Strategy::kEndpoint;
  proxy.RegisterEndpoint(endpoint);
  const auto port = proxy.GetEndpointPort(endpoint);

  const auto app =
      stonks::server::V1Strategy{*port, ccutils::MakeSp<stonks::V1Strategy>()};
  static_cast<void>(getchar());
}