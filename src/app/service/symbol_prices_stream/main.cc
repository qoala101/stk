#include <fmt/core.h>

#include <boost/di.hpp>

#include "cli_app.h"
#include "core_i_symbols_db.h"
#include "core_symbol_prices_stream.h"
#include "core_types.h"
#include "di_bind_interface_to_implementation.h"
#include "di_bind_type_to_value.h"
#include "di_make_injector.h"
#include "network_types.h"
#include "service_log_spdlog_injector.h"
#include "service_network_restsdk_injector.h"
#include "service_symbols_db.h"

auto main(int argc, const char *const *argv) -> int {
  stonks::cli::App{argc, argv}.Run([](const auto &options) {
    const auto injector = stonks::di::MakeInjector(
        stonks::service::injectors::CreateNetworkRestsdkInjector(),
        stonks::service::injectors::CreateLogSpdlogInjector(),
        stonks::di::BindTypeToValue<stonks::core::Symbol>(
            stonks::core::Symbol{options.GetOptionOr("symbol", "BTCUSDT")}),
        stonks::di::BindTypeToValue<stonks::network::Uri>(
            stonks::network::Uri{fmt::format(
                "http://{}:{}", options.GetOptionOr("sdb_host", "0.0.0.0"),
                options.GetOptionOr("sdb_port", 6506))}),
        stonks::di::BindInterfaceToImplementation<
            stonks::core::ISymbolsDb, stonks::service::SymbolsDb>());

    return injector.template create<stonks::core::SymbolPricesStream>();
  });
}