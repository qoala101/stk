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
#include "service_create_log_spdlog_injector.h"
#include "service_create_network_restsdk_injector.h"
#include "core_i_symbols_db.h"
#include "core_i_symbols_db_updater.h"
#include "core_sps_stream_factory.h"
#include "core_symbol_price_streams.h"
#include "cpp_not_null.h"
#include "cpp_share.h"
#include "di_auto_injectable.h"
#include "di_bind_type_to_factory_function.h"
#include "di_bind_value_type_to_value.h"
#include "di_enable_nn_pointers.h"
#include "di_make_injector.h"
#include "network_i_rest_request_sender.h"
#include "network_ts_rest_request_sender.h"
#include "networkx_make_server_for.h"
#include "service_client_options.h"
#include "service_create_client_injector.h"
#include "service_create_server_injector.h"
#include "service_sdb_traits.h"  // IWYU pragma: keep
#include "service_sdu_traits.h"  // IWYU pragma: keep
#include "service_server_options.h"
#include "service_sps_traits.h"  // IWYU pragma: keep
#include "service_symbols_db.h"
#include "service_symbols_db_updater.h"

auto main(int argc, const char *const *argv) -> int {
  auto options = stonks::cli::Options{};

  const auto server_options =
      stonks::service::ServerOptions<stonks::core::SymbolPriceStreams>{options};
  const auto symbols_db_client_options =
      stonks::service::ClientOptions<stonks::core::ISymbolsDb>{options};
  const auto symbols_db_updater_client_options =
      stonks::service::ClientOptions<stonks::core::ISymbolsDbUpdater>{options};
  auto symbols = options.AddOption(
      "--symbols", std::vector<std::string>{"BTCUSDT", "ETHUSDT", "BNBUSDT"});
  const auto reattempt_interval = options.AddOption(
      "--reattempt_interval", absl::ToInt64Milliseconds(absl::Minutes(1)));

  const auto app = stonks::cli::App{argc, argv, options};
  const auto injector = stonks::cpp::Share(stonks::di::MakeInjector(
      stonks::service::CreateNetworkRestsdkInjector(),
      stonks::service::CreateLogSpdlogInjector(),

      stonks::service::CreateServerInjector<stonks::core::SymbolPriceStreams>(
          server_options),
      stonks::service::CreateClientInjector<stonks::service::SymbolsDb>(
          symbols_db_client_options),
      stonks::service::CreateClientInjector<stonks::service::SymbolsDbUpdater>(
          symbols_db_updater_client_options),

      stonks::di::BindValueTypeToValue(
          absl::Milliseconds(*reattempt_interval))));

  app.Run([&injector, &symbols]() {
    auto auto_injectable = stonks::di::AutoInjectable{injector};

    return stonks::networkx::MakeServerFor<stonks::core::SymbolPriceStreams>(
        stonks::cpp::MakeNnUp<stonks::core::SymbolPriceStreams>(
            stonks::core::SymbolPriceStreams{
                *symbols | ranges::views::transform([](auto &symbol) {
                  return stonks::core::Symbol{std::move(symbol)};
                }) | ranges::to_vector,
                auto_injectable, auto_injectable}),
        auto_injectable, auto_injectable);
  });
}