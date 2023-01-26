// clang-format off
#include "service_blsh_json_conversions.h"  // IWYU pragma: keep
// clang-format on

#include <boost/di.hpp>
#include <string>

#include "cli_app.h"
#include "cli_options.h"
#include "core_buy_low_sell_high.h"
#include "core_i_symbols_db.h"
#include "di_call_with_injected_args.h"
#include "di_make_injector.h"
#include "network_json_common_conversions.h"  // IWYU pragma: keep
#include "network_rest_server.h"
#include "networkx_make_server_for.h"
#include "service_blsh_traits.h"  // IWYU pragma: keep
#include "service_client_options.h"
#include "service_inj_client_server.h"
#include "service_inj_log_spdlog.h"
#include "service_inj_network_restsdk.h"
#include "service_inj_ts_symbols_db_override.h"
#include "service_sdb_traits.h"  // IWYU pragma: keep
#include "service_server_options.h"
#include "service_symbols_db.h"

namespace stonks::service::aue {
void Main(int argc, const char *const *argv) {
  auto options = cli::Options();

  const auto server_options = ServerOptions<core::BuyLowSellHigh>{options};
  const auto symbols_db_client_options =
      ClientOptions<core::ISymbolsDb>{options};

  const auto app = cli::App{argc, argv, options};
  auto base_injector = di::MakeInjector(
      inj::CreateNetworkRestsdkInjector(), inj::CreateLogSpdlogInjector(),
      inj::CreateServerInjector<core::BuyLowSellHigh>(server_options),
      inj::CreateClientInjector<SymbolsDb>(symbols_db_client_options));
  const auto injector =
      inj::ts::OverrideThreadSafeSymbolsDbInjector(base_injector);

  app.Run([&injector]() {
    return di::CallWithInjectedArgs(
        networkx::MakeServerFor<core::BuyLowSellHigh>, injector);
  });
}
}  // namespace stonks::service::aue

auto main(int argc, const char *const *argv) -> int {
  stonks::service::aue::Main(argc, argv);
}