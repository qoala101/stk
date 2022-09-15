#include <boost/di.hpp>
#include <cstdio>
#include <memory>
#include <tuple>

#include "app_injectors.h"
#include "app_pnd_items_interface_server.h"
#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "network_types.h"
#include "not_null.hpp"

auto main(int /*unused*/, char* /*unused*/[]) -> int {
  // clang-format off
  const auto injector = make_injector(
    stonks::app::injectors::MakeNetworkInjector(),
    stonks::app::injectors::MakeNosqldbInjector(),
    stonks::app::injectors::MakeLogInjector(),

    boost::di::bind<stonks::network::Uri>.to(stonks::network::Uri{"http://localhost:6506/Entity"})
  );
  // clang-format on

  const auto logger =
      injector.create<stonks::cpp::NnUp<stonks::log::ILogger>>();
  logger->LogImportantEvent("Started: public_nosql_db");

  const auto server = injector.create<stonks::app::pnd::ItemsInterfaceServer>();

  std::ignore = getchar();
  logger->LogImportantEvent("Ended: public_nosql_db");
}