#include <absl/time/time.h>
#include <fmt/core.h>

#include <cstdint>

#include "cli_app.h"
#include "core_i_symbols_db.h"
#include "core_symbols_info_updater.h"
#include "di_bind_interface_to_implementation.h"
#include "di_bind_type_to_factory_function.h"
#include "di_bind_type_to_value.h"
#include "di_make_injector.h"
#include "network_types.h"
#include "service_log_spdlog_injector.h"
#include "service_network_restsdk_injector.h"
#include "service_symbols_db.h"
#include "service_symbols_db_injector.h"

auto main(int argc, const char *const *argv) -> int {
  stonks::cli::App{argc, argv}.Run([](const auto &options) {
    const auto update_interval = options.GetOptionOr(
        "update_interval", int64_t{absl::ToInt64Milliseconds(absl::Hours(1))});

    const auto injector = stonks::di::MakeInjector(
        stonks::service::injectors::CreateNetworkRestsdkInjector(),
        stonks::service::injectors::CreateLogSpdlogInjector(),
        stonks::service::CreateSymbolsDbInjector(options),

        stonks::di::BindTypeToValue<int64_t>(update_interval),
        stonks::di::BindTypeToFactoryFunction<absl::Duration,
                                              +[](int64_t milliseconds) {
                                                return absl::Milliseconds(
                                                    milliseconds);
                                              }>());

    return injector.template create<stonks::core::SymbolsInfoUpdater>();
  });
}