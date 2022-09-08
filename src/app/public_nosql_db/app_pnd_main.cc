#include <cstdio>
#include <iostream>
#include <string_view>
#include <tuple>

#include "app_pnd_items_interface_server.h"

auto main(int /*unused*/, const char* /*unused*/[]) -> int {
  std::cout << "Started: public_nosql_db\n";
  const auto server =
      stonks::app::pnd::ItemsInterfaceServer{"http://localhost:6506/Entity"};
  std::ignore = getchar();
  std::cout << "Ended: public_nosql_db\n";
}