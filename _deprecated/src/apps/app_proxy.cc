#include <stdio.h>

#include "cpp_not_null.h"

#include "proxy.h"
#include "server_proxy.h"

auto main(int /*unused*/, const char* /*unused*/[]) -> int {
  const auto app =
      stonks::server::Proxy{cpp::MakeSp<stonks::network::Proxy>()};
  static_cast<void>(getchar());
}