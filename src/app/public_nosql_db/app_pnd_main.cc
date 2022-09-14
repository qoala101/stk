#include <boost/di.hpp>
#include <cstdio>
#include <iostream>
#include <string>
#include <tuple>

#include "app_injectors.h"
#include "app_pnd_items_interface_server.h"

auto main(int /*unused*/, char* /*unused*/[]) -> int {
  std::cout << "Started: public_nosql_db\n";

  // clang-format off
  const auto injector = make_injector(
    stonks::app::injectors::MakeNetworkInjector(),
    stonks::app::injectors::MakeNosqldbInjector(),
    stonks::app::injectors::MakeLogInjector(),

    boost::di::bind<std::string>.to("http://localhost:6506/Entity")
  );
  // clang-format on

  const auto server = injector.create<stonks::app::pnd::ItemsInterfaceServer>();

  std::ignore = getchar();
  std::cout << "Ended: public_nosql_db\n";
}