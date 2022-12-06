#include <utility>

#include "cli_app.h"
#include "core_symbol_prices_stream.h"
#include "core_types.h"
#include "di_bind_type_to_value.h"
#include "di_make_injector.h"
#include "service_log_spdlog_injector.h"
#include "service_network_restsdk_injector.h"
#include "service_symbols_db_injector.h"

auto main(int argc, const char *const *argv) -> int {
  stonks::cli::App{argc, argv}.Run([](const auto &options) {
    auto symbol = options.GetOptionOr("symbol", "BTCUSDT");

    const auto injector = stonks::di::MakeInjector(
        stonks::service::injectors::CreateNetworkRestsdkInjector(),
        stonks::service::injectors::CreateLogSpdlogInjector(),
        stonks::service::CreateSymbolsDbInjector(options),

        stonks::di::BindTypeToValue<stonks::core::Symbol>(
            stonks::core::Symbol{std::move(symbol)}));

    return injector.template create<stonks::core::SymbolPricesStream>();
  });
}