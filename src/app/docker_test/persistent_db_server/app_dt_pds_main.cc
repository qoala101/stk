#include <fmt/core.h>

#include <memory>

#include "app_log_spdlog_injector.h"
#include "app_network_restsdk_injector.h"
#include "app_sqldb_sqlite_injector.h"
#include "cli_app.h"
#include "di_bind_type_to_value.h"
#include "di_make_injector.h"
#include "network_types.h"
#include "sqlite_types.h"

auto main(int argc, const char* const* argv) -> int {
  stonks::cli::App{argc, argv}.Run([](const auto& options) {
    const auto injector = stonks::di::MakeInjector(
        stonks::app::injectors::CreateNetworkRestsdkInjector(),
        stonks::app::injectors::CreateSqldbSqliteInjector(),
        stonks::app::injectors::CreateLogSpdlogInjector(),
        stonks::di::BindTypeToValue<stonks::network::Uri>(
            stonks::network::Uri{fmt::format(
                "http://0.0.0.0:{}", options.GetOptionOr("port", 6506))}),
        stonks::di::BindTypeToValue<stonks::sqlite::FilePath>(
            stonks::sqlite::FilePath{"persistent_db_server.db"}));

    // return injector.create<stonks::app::dt::pds::AppServer>();
    return 0;
  });
}