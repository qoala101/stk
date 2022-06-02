#include "server_proxy.h"

auto main(int /*unused*/, const char* /*unused*/[]) -> int {
  const auto app =
      stonks::ProxyServer{std::make_shared<stonks::network::Proxy>()};
  static_cast<void>(getchar());
}