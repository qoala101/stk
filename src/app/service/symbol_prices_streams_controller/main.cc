// clang-format off
#include "common_json_conversions.h"  // IWYU pragma: keep
// clang-format on

#include <boost/di.hpp>

#include "cli_app.h"
#include "cli_options.h"
#include "common_log_spdlog_injector.h"
#include "common_network_restsdk_injector.h"
#include "core_i_symbol_prices_streams_controller.h"
#include "core_symbol_prices_streams_controller.h"
#include "di_call_with_injected_args.h"
#include "di_make_injector.h"
#include "network_json_common_conversions.h"  // IWYU pragma: keep
#include "network_rest_server.h"
#include "networkx_make_server_for.h"
#include "service_create_server_injector.h"
#include "service_server_options.h"
#include "service_spsc_traits.h"  // IWYU pragma: keep

auto main(int argc, const char *const *argv) -> int {
  auto options = stonks::cli::Options{};

  const auto server_options = stonks::service::ServerOptions<
      stonks::core::ISymbolPricesStreamsController>{options};

  const auto app = stonks::cli::App{argc, argv, options};
  const auto injector = stonks::di::MakeInjector(
      stonks::common::CreateNetworkRestsdkInjector(),
      stonks::common::CreateLogSpdlogInjector(),

      stonks::service::CreateServerInjector<
          stonks::core::SymbolPricesStreamsController>(server_options));

  app.Run([&injector]() {
    return stonks::di::CallWithInjectedArgs(
        stonks::networkx::MakeServerFor<
            stonks::core::ISymbolPricesStreamsController>,
        injector);
  });
}