// clang-format off
#include "service_json_conversions.h"   // IWYU pragma: keep
// clang-format on

#include <fmt/core.h>

#include <string>
#include <utility>

#include "cli_app.h"
#include "core_i_symbols_db.h"
#include "core_symbols_db.h"
#include "cpp_not_null.h"
#include "di_bind_interface_to_implementation.h"
#include "di_bind_type_to_factory_function.h"
#include "di_bind_type_to_value.h"
#include "di_make_injector.h"
#include "network_json_common_conversions.h"  // IWYU pragma: keep
#include "network_rest_server.h"
#include "network_rest_server_builder.h"
#include "network_types.h"
#include "networkx_make_server_for.h"
#include "service_log_spdlog_injector.h"
#include "service_network_restsdk_injector.h"
#include "service_sqldb_sqlite_injector.h"
#include "service_symbols_db_traits.h"  // IWYU pragma: keep
#include "sqlite_types.h"

auto main(int argc, const char *const *argv) -> int {
  stonks::cli::App{argc, argv}.Run([](const auto &options) {
    const auto port = options.GetOptionOr("port", 6506);
    auto db_file_path = options.GetOptionOr("db_file_path", "symbols_db.db");

    const auto injector = stonks::di::MakeInjector(
        stonks::service::injectors::CreateNetworkRestsdkInjector(),
        stonks::service::injectors::CreateSqldbSqliteInjector(),
        stonks::service::injectors::CreateLogSpdlogInjector(),

        stonks::di::BindTypeToValue<stonks::network::Uri>(
            stonks::network::Uri{fmt::format("http://0.0.0.0:{}", port)}),
        stonks::di::BindTypeToValue<stonks::sqlite::FilePath>(
            stonks::sqlite::FilePath{std::move(db_file_path)}),
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