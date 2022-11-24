// clang-format off
#include "service_json_conversions.h"
// clang-format on

#include <fmt/core.h>

#include <memory>

#include "cli_app.h"
#include "core_symbols_db.h"
#include "cpp_not_null.h"
#include "di_bind_type_to_factory_function.h"
#include "di_bind_type_to_value.h"
#include "di_make_injector.h"
#include "network_rest_server.h"
#include "network_rest_server_builder.h"
#include "network_types.h"
#include "networkx_make_server_for.h"
#include "service_log_spdlog_injector.h"
#include "service_network_restsdk_injector.h"
#include "service_sqldb_sqlite_injector.h"
#include "service_symbols_db_traits.h"  // IWYU pragma: keep

auto main(int argc, const char *const *argv) -> int {
  stonks::cli::App{argc, argv}.Run([](const auto &options) {
    const auto injector = stonks::di::MakeInjector(
        stonks::service::injectors::CreateNetworkRestsdkInjector(),
        stonks::service::injectors::CreateSqldbSqliteInjector(),
        stonks::service::injectors::CreateLogSpdlogInjector(),
        stonks::di::BindTypeToValue<stonks::network::Uri>(
            stonks::network::Uri{fmt::format(
                "http://0.0.0.0:{}", options.GetOptionOr("port", 6506))}),
        stonks::di::BindTypeToValue<stonks::sqlite::FilePath>(
            stonks::sqlite::FilePath{"symbols_db.db"}),
        stonks::di::BindInterfaceToImplementation<stonks::core::ISymbolsDb,
                                                  stonks::core::SymbolsDb>(),
        stonks::di::BindTypeToFactoryFunction<
            stonks::network::RestServer,
            +[](const stonks::cpp::NnSp<stonks::core::ISymbolsDb> &target,
                stonks::network::RestServerBuilder server_builder) {
              return stonks::networkx::MakeServerFor(target, server_builder);
            }>());

    return injector.template create<stonks::network::RestServer>();
  });
}