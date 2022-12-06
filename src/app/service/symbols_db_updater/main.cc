#include <absl/time/time.h>

#include <cstdint>

#include "cli_app.h"
#include "core_sdbu_old_prices_deleter.h"
#include "core_sdbu_symbols_info_updater.h"
#include "core_symbols_db_updater.h"
#include "di_bind_type_to_value.h"
#include "di_make_injector.h"
#include "service_log_spdlog_injector.h"
#include "service_network_restsdk_injector.h"
#include "service_symbols_db_injector.h"

auto main(int argc, const char *const *argv) -> int {
  stonks::cli::App{argc, argv}.Run([](const auto &options) {
    const auto update_symbols_info_interval =
        options.GetOptionOr("update_symbols_info_interval",
                            int64_t{absl::ToInt64Milliseconds(absl::Hours(1))});
    const auto delete_old_prices_interval = options.GetOptionOr(
        "delete_old_prices_interval",
        int64_t{absl::ToInt64Milliseconds(absl::Minutes(1))});
    const auto keep_prices_for = options.GetOptionOr(
        "keep_prices_for", int64_t{absl::ToInt64Milliseconds(absl::Hours(1))});

    const auto injector = stonks::di::MakeInjector(
        stonks::service::injectors::CreateNetworkRestsdkInjector(),
        stonks::service::injectors::CreateLogSpdlogInjector(),
        stonks::service::CreateSymbolsDbInjector(options),

        stonks::di::BindTypeToValue<absl::Duration>(
            absl::Milliseconds(keep_prices_for)));

    return stonks::core::SymbolsDbUpdater{
        injector.template create<stonks::core::sdbu::SymbolsInfoUpdater>(),
        absl::Milliseconds(update_symbols_info_interval),
        injector.template create<stonks::core::sdbu::OldPricesDeleter>(),
        absl::Milliseconds(delete_old_prices_interval)};
  });
}