#include <absl/time/time.h>

#include <boost/di.hpp>
#include <cstdint>
#include <string>
#include <variant>

#include "cli_app.h"
#include "cli_option.h"
#include "cli_options.h"
#include "common_create_log_spdlog_injector.h"
#include "common_create_network_restsdk_injector.h"
#include "core_i_symbol_prices_streams_controller.h"
#include "core_i_symbols_db.h"
#include "core_symbol_prices_stream.h"
#include "di_bind_value_type_to_value.h"
#include "di_make_injector.h"
#include "service_client_options.h"
#include "service_create_client_injector.h"
#include "service_sdb_traits.h"   // IWYU pragma: keep
#include "service_spsc_traits.h"  // IWYU pragma: keep
#include "service_symbol_prices_streams_controller.h"
#include "service_symbols_db.h"

auto main(int argc, const char *const *argv) -> int {
  auto options = stonks::cli::Options{};

  const auto symbols_db_client_options =
      stonks::service::ClientOptions<stonks::core::ISymbolsDb>{options};
  const auto controller_client_options = stonks::service::ClientOptions<
      stonks::core::ISymbolPricesStreamsController>{options};
  const auto reattempt_interval = options.AddOption(
      "--reattempt_interval", absl::ToInt64Milliseconds(absl::Minutes(1)));

  const auto app = stonks::cli::App{argc, argv, options};
  const auto injector = stonks::di::MakeInjector(
      stonks::common::CreateNetworkRestsdkInjector(),
      stonks::common::CreateLogSpdlogInjector(),

      stonks::service::CreateClientInjector<stonks::service::SymbolsDb>(
          symbols_db_client_options),
      stonks::service::CreateClientInjector<
          stonks::service::SymbolPricesStreamsController>(
          controller_client_options),

      stonks::di::BindValueTypeToValue(
          absl::Milliseconds(*reattempt_interval)));

  app.Run([&injector]() {
    return injector.template create<stonks::core::SymbolPricesStream>();
  });
}