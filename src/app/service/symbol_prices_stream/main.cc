#include <absl/time/time.h>

#include <boost/di.hpp>
#include <cstdint>
#include <string>
#include <variant>

#include "cli_app.h"
#include "cli_option.h"
#include "cli_options.h"
#include "core_i_symbol_prices_streams_controller.h"
#include "core_i_symbols_db.h"
#include "core_symbol_prices_stream.h"
#include "di_bind_value_type_to_value.h"
#include "di_make_injector.h"
#include "service_create_service_client_injector.h"
#include "service_log_spdlog_injector.h"
#include "service_network_restsdk_injector.h"
#include "service_options.h"
#include "service_sdb_client.h"   // IWYU pragma: keep
#include "service_sdb_traits.h"   // IWYU pragma: keep
#include "service_spsc_client.h"  // IWYU pragma: keep
#include "service_spsc_traits.h"  // IWYU pragma: keep

auto main(int argc, const char *const *argv) -> int {
  auto options = stonks::cli::Options{};

  const auto reattempt_interval = options.AddOption(
      "--reattempt_interval", absl::ToInt64Milliseconds(absl::Minutes(1)));
  const auto symbols_db_service_options =
      stonks::service::ServiceOptions<stonks::core::ISymbolsDb>{options};
  const auto controller_service_options = stonks::service::ServiceOptions<
      stonks::core::ISymbolPricesStreamsController>{options};

  const auto app = stonks::cli::App{argc, argv, options};
  const auto injector = stonks::di::MakeInjector(
      stonks::service::injectors::CreateNetworkRestsdkInjector(),
      stonks::service::injectors::CreateLogSpdlogInjector(),
      stonks::service::CreateServiceClientInjector(symbols_db_service_options),
      stonks::service::CreateServiceClientInjector(controller_service_options),

      stonks::di::BindValueTypeToValue(
          absl::Milliseconds(*reattempt_interval)));

  app.Run([&injector]() {
    return injector.template create<stonks::core::SymbolPricesStream>();
  });
}