#include <absl/time/time.h>

#include <boost/di.hpp>
#include <cstdint>
#include <memory>
#include <utility>

#include "cli_app.h"
#include "cli_options.h"
#include "core_symbol_prices_stream.h"
#include "core_types.h"
#include "di_bind_type_to_value.h"
#include "di_make_injector.h"
#include "service_log_spdlog_injector.h"
#include "service_network_restsdk_injector.h"
#include "service_symbols_db_injector.h"

auto main(int argc, const char *const *argv) -> int {
  auto options = stonks::cli::Options{};

  auto symbol = options.AddOption("--symbol", "BTCUSDT");
  const auto reattempt_interval = options.AddOption(
      "--reattempt_interval", absl::ToInt64Milliseconds(absl::Minutes(1)));
  const auto symbols_db_options = stonks::service::SymbolsDbOptions{options};

  const auto app = stonks::cli::App{argc, argv, options};
  const auto injector = stonks::di::MakeInjector(
      stonks::service::injectors::CreateNetworkRestsdkInjector(),
      stonks::service::injectors::CreateLogSpdlogInjector(),
      stonks::service::CreateSymbolsDbInjector(symbols_db_options),

      stonks::di::BindTypeToValue<stonks::core::Symbol>(
          stonks::core::Symbol{std::move(*symbol)}),
      stonks::di::BindTypeToValue<absl::Duration>(
          absl::Milliseconds(*reattempt_interval)));

  app.Run([&injector]() {
    return injector.template create<stonks::core::SymbolPricesStream>();
  });
}