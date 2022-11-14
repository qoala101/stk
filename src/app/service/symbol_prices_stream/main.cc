#include <fmt/core.h>

#include <utility>

#include "cli_app.h"
#include "core_i_symbols_db.h"
#include "core_sps_endpoints.h"
#include "core_symbol_prices_stream.h"
#include "core_types.h"
#include "di_bind_interface_to_implementation.h"
#include "di_bind_type_to_factory_function.h"
#include "di_bind_type_to_value.h"
#include "di_make_injector.h"
#include "network_typed_ws_endpoint.h"
#include "network_types.h"
#include "service_log_spdlog_injector.h"
#include "service_network_restsdk_injector.h"
#include "service_symbols_db.h"

auto main(int argc, const char *const *argv) -> int {
  stonks::cli::App{argc, argv}.Run([](const auto &options) {
    struct TypedWsEndpointFactory {
      auto operator() [[nodiscard]] (stonks::core::Symbol symbol) {
        return stonks::app::sps::endpoints::BinanceSymbolBookTickerStream(
            std::move(symbol));
      }
    };

    const auto injector = stonks::di::MakeInjector(
        stonks::app::injectors::CreateNetworkRestsdkInjector(),
        stonks::app::injectors::CreateLogSpdlogInjector(),
        stonks::di::BindTypeToValue<stonks::core::Symbol>(
            stonks::core::Symbol{options.GetOptionOr("symbol", "BTCUSDT")}),
        stonks::di::BindTypeToValue<stonks::network::Uri>(
            stonks::network::Uri{fmt::format(
                "http://{}:{}", options.GetOptionOr("sdb_host", "0.0.0.0"),
                options.GetOptionOr("sdb_port", 6506))}),
        stonks::di::BindTypeToFactoryFunction<stonks::network::TypedWsEndpoint,
                                              TypedWsEndpointFactory,
                                              stonks::core::Symbol>(),
        stonks::di::BindInterfaceToImplementation<
            stonks::app::sdb::IApp,
            stonks::app::a::Connection<stonks::app::sdb::IApp>>());

    return injector.template create<stonks::app::sps::App>();
  });
}