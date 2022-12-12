#include <absl/time/time.h>

#include <boost/di.hpp>
#include <cstdint>
#include <memory>

#include "cli_app.h"
#include "cli_options.h"
#include "core_sdbu_old_prices_deleter.h"
#include "core_sdbu_symbols_info_updater.h"
#include "core_symbols_db_updater.h"
#include "di_bind_type_to_value.h"
#include "di_make_injector.h"
#include "service_log_spdlog_injector.h"
#include "service_network_restsdk_injector.h"
#include "service_symbols_db_injector.h"
#include "service_symbols_db_options.h"

auto main(int argc, const char *const *argv) -> int {
  auto options = stonks::cli::Options();

  const auto update_symbols_info_interval =
      options.AddOption("--update_symbols_info_interval",
                        absl::ToInt64Milliseconds(absl::Hours(1)));
  const auto delete_old_prices_interval =
      options.AddOption("--delete_old_prices_interval",
                        absl::ToInt64Milliseconds(absl::Hours(1)));
  const auto keep_prices_for_duration = options.AddOption(
      "--keep_prices_for_duration", absl::ToInt64Milliseconds(absl::Hours(1)));
  const auto reattempt_interval = options.AddOption(
      "--reattempt_interval", absl::ToInt64Milliseconds(absl::Minutes(1)));
  const auto symbols_db_options = stonks::service::SymbolsDbOptions{options};

  const auto app = stonks::cli::App{argc, argv, options};
  const auto injector = stonks::di::MakeInjector(
      stonks::service::injectors::CreateNetworkRestsdkInjector(),
      stonks::service::injectors::CreateLogSpdlogInjector(),
      stonks::service::CreateSymbolsDbInjector(symbols_db_options),

      stonks::di::BindTypeToValue<absl::Duration>(
          absl::Milliseconds(*keep_prices_for_duration)));

  app.Run([&injector, &update_symbols_info_interval,
           &delete_old_prices_interval, &reattempt_interval]() {
    return stonks::core::SymbolsDbUpdater{
        absl::Milliseconds(*update_symbols_info_interval),
        injector.template create<stonks::core::sdbu::SymbolsInfoUpdater>(),
        absl::Milliseconds(*delete_old_prices_interval),
        injector.template create<stonks::core::sdbu::OldPricesDeleter>(),
        absl::Milliseconds(*reattempt_interval)};
  });
}