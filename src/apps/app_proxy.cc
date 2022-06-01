#include "proxy_client_server.h"

auto main(int /*unused*/, const char* /*unused*/[]) -> int {
  const auto app = stonks::ProxyServer{};
  static_cast<void>(getchar());
}