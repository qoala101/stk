// clang-format off
#include "service_blsh_json_conversions.h"  // IWYU pragma: keep
// clang-format on

#include <boost/di.hpp>
#include <string>

#include "cli_app.h"
#include "cli_options.h"
#include "common_log_spdlog_injector.h"
#include "common_network_restsdk_injector.h"
#include "core_buy_low_sell_high.h"
#include "core_i_symbols_db.h"
#include "di_call_with_injected_args.h"
#include "di_make_injector.h"
#include "network_json_common_conversions.h"  // IWYU pragma: keep
#include "network_rest_server.h"
#include "networkx_make_server_for.h"
#include "service_blsh_traits.h"  // IWYU pragma: keep
#include "service_client_options.h"
#include "service_create_client_injector.h"
#include "service_create_server_injector.h"
#include "service_sdb_traits.h"  // IWYU pragma: keep
#include "service_server_options.h"
#include "service_symbols_db.h"

auto main(int argc, const char *const *argv) -> int {
  auto options = stonks::cli::Options();

  const auto server_options =
      stonks::service::ServerOptions<stonks::core::BuyLowSellHigh>{options};
  const auto symbols_db_client_options =
      stonks::service::ClientOptions<stonks::core::ISymbolsDb>{options};

  const auto app = stonks::cli::App{argc, argv, options};
  const auto injector = stonks::di::MakeInjector(
      stonks::common::CreateNetworkRestsdkInjector(),
      stonks::common::CreateLogSpdlogInjector(),

      stonks::service::CreateServerInjector<stonks::core::BuyLowSellHigh>(
          server_options),
      stonks::service::CreateClientInjector<stonks::service::SymbolsDb>(
          symbols_db_client_options));

  app.Run([&injector]() {
    return stonks::di::CallWithInjectedArgs(
        stonks::networkx::MakeServerFor<stonks::core::BuyLowSellHigh>,
        injector);
  });
}