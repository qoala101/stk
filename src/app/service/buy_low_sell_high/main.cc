// clang-format off
#include "service_blsh_json_conversions.h"  // IWYU pragma: keep
// clang-format on

#include <fmt/core.h>

#include <boost/di.hpp>
#include <string>
#include <variant>

#include "cli_app.h"
#include "cli_option.h"
#include "cli_options.h"
#include "core_buy_low_sell_high.h"
#include "core_i_symbols_db.h"
#include "di_bind_value_type_to_value.h"
#include "di_call_with_injected_args.h"
#include "di_enable_nn_pointers.h"
#include "di_make_injector.h"
#include "network_json_common_conversions.h"  // IWYU pragma: keep
#include "network_rest_server.h"
#include "networkx_make_server_for.h"
#include "networkx_uri.h"
#include "service_blsh_traits.h"  // IWYU pragma: keep
#include "service_create_service_client_injector.h"
#include "service_log_spdlog_injector.h"
#include "service_network_restsdk_injector.h"
#include "service_options.h"
#include "service_sdb_client.h"  // IWYU pragma: keep
#include "service_sdb_traits.h"  // IWYU pragma: keep

auto main(int argc, const char *const *argv) -> int {
  auto options = stonks::cli::Options();

  const auto port = options.AddOption("--port", 30000);
  const auto symbols_db_service_options =
      stonks::service::ServiceOptions<stonks::core::ISymbolsDb>{options};

  const auto app = stonks::cli::App{argc, argv, options};
  const auto injector = stonks::di::MakeInjector(
      stonks::service::injectors::CreateNetworkRestsdkInjector(),
      stonks::service::injectors::CreateLogSpdlogInjector(),
      stonks::service::CreateServiceClientInjector(symbols_db_service_options),

      stonks::di::BindValueTypeToValue(
          stonks::networkx::Uri<stonks::core::BuyLowSellHigh>{
              fmt::format("http://0.0.0.0:{}", *port)}),
      stonks::di::EnableNnPointers<stonks::core::BuyLowSellHigh>());

  app.Run([&injector]() {
    return stonks::di::CallWithInjectedArgs(
        stonks::networkx::MakeServerFor<stonks::core::BuyLowSellHigh>,
        injector);
  });
}