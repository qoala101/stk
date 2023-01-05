#include <absl/time/time.h>

#include <boost/di.hpp>
#include <cstdint>
#include <iostream>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <string>
#include <variant>

#include "cli_app.h"
#include "cli_option.h"
#include "cli_options.h"
#include "common_create_log_spdlog_injector.h"
#include "common_create_network_restsdk_injector.h"
#include "core_i_symbols_db.h"
#include "core_symbol_price_streams.h"
#include "cpp_not_null.h"
#include "di_auto_injectable.h"
#include "di_bind_value_type_to_value.h"
#include "di_make_injector.h"
#include "networkx_make_server_for.h"
#include "service_client_options.h"
#include "service_create_client_injector.h"
#include "service_create_server_injector.h"
#include "service_sdb_traits.h"  // IWYU pragma: keep
#include "service_server_options.h"
#include "service_sps_traits.h"  // IWYU pragma: keep
#include "service_symbols_db.h"

auto main(int argc, const char *const *argv) -> int {
  auto options = stonks::cli::Options{};

  const auto server_options =
      stonks::service::ServerOptions<stonks::core::SymbolPriceStreams>{options};
  const auto symbols_db_client_options =
      stonks::service::ClientOptions<stonks::core::ISymbolsDb>{options};
  auto symbols = options.AddOption(
      "--symbols", std::vector<std::string>{"BTCUSDT", "ETHUSDT"});
  const auto reattempt_interval = options.AddOption(
      "--reattempt_interval", absl::ToInt64Milliseconds(absl::Minutes(1)));

  const auto app = stonks::cli::App{argc, argv, options};
  const auto injector = stonks::di::MakeInjector(
      stonks::common::CreateNetworkRestsdkInjector(),
      stonks::common::CreateLogSpdlogInjector(),

      stonks::service::CreateServerInjector<stonks::core::SymbolPriceStreams>(
          server_options),
      stonks::service::CreateClientInjector<stonks::service::SymbolsDb>(
          symbols_db_client_options),

      stonks::di::BindValueTypeToValue(
          absl::Milliseconds(*reattempt_interval)));

  app.Run([injector = stonks::cpp::AssumeNn(&injector), &symbols]() {
    return stonks::networkx::MakeServerFor<stonks::core::SymbolPriceStreams>(
        stonks::cpp::MakeNnSp<stonks::core::SymbolPriceStreams>(
            stonks::core::SymbolPriceStreams{
                *symbols | ranges::views::transform([](auto &symbol) {
                  return stonks::core::Symbol{std::move(symbol)};
                }) | ranges::to_vector,
                stonks::di::AutoInjectable{injector},
                stonks::di::AutoInjectable{injector},
                stonks::di::AutoInjectable{injector}}),
        stonks::di::AutoInjectable{injector},
        stonks::di::AutoInjectable{injector});
  });
}