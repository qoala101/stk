// clang-format off
#include "service_buy_low_sell_high_json_conversions.h"  // IWYU pragma: keep
// clang-format on

#include <absl/time/time.h>

#include <boost/di.hpp>
#include <cstdint>
#include <memory>

#include "cli_app.h"
#include "cli_options.h"
#include "core_buy_low_sell_high.h"
#include "di_enable_nn_pointers.h"
#include "di_make_injector.h"
#include "network_i_rest_request_receiver.h"
#include "network_json_common_conversions.h"  // IWYU pragma: keep
#include "network_types.h"
#include "networkx_make_server_for.h"
#include "service_buy_low_sell_high_traits.h"  // IWYU pragma: keep
#include "service_log_spdlog_injector.h"
#include "service_network_restsdk_injector.h"
#include "service_symbols_db_injector.h"
#include "service_symbols_db_options.h"

auto main(int argc, const char *const *argv) -> int {
  auto options = stonks::cli::Options();

  const auto port = options.AddOption("--port", 6507);
  const auto symbols_db_options = stonks::service::SymbolsDbOptions{options};

  const auto app = stonks::cli::App{argc, argv, options};
  const auto injector = stonks::di::MakeInjector(
      stonks::service::injectors::CreateNetworkRestsdkInjector(),
      stonks::service::injectors::CreateLogSpdlogInjector(),
      stonks::service::CreateSymbolsDbInjector(symbols_db_options),

      stonks::di::EnableNnPointers<stonks::core::BuyLowSellHigh>());

  app.Run([&injector, &port]() {
    return stonks::networkx::MakeServerFor(
        injector
            .template create<stonks::cpp::NnSp<stonks::core::BuyLowSellHigh>>(),
        stonks::network::RestServerBuilder{
            stonks::network::Uri{fmt::format("http://0.0.0.0:{}", *port)},
            injector.template create<
                stonks::cpp::NnUp<stonks::network::IRestRequestReceiver>>()});
  });
}