// clang-format off
#include "common_json_conversions.h"  // IWYU pragma: keep
// clang-format on

#include <fmt/core.h>

#include <boost/di.hpp>
#include <string>
#include <utility>
#include <variant>

#include "cli_app.h"
#include "cli_option.h"
#include "cli_options.h"
#include "common_log_spdlog_injector.h"
#include "common_network_restsdk_injector.h"
#include "common_sqldb_sqlite_injector.h"
#include "core_i_symbols_db.h"
#include "core_symbols_db.h"
#include "di_bind_value_type_to_value.h"
#include "di_call_with_injected_args.h"
#include "di_make_injector.h"
#include "network_json_common_conversions.h"  // IWYU pragma: keep
#include "network_rest_server.h"
#include "networkx_make_server_for.h"
#include "networkx_uri.h"
#include "service_create_server_injector.h"
#include "service_sdb_traits.h"  // IWYU pragma: keep
#include "service_server_options.h"
#include "sqlite_types.h"

auto main(int argc, const char *const *argv) -> int {
  auto options = stonks::cli::Options{};

  const auto server_options =
      stonks::service::ServerOptions<stonks::core::ISymbolsDb>{options};
  auto db_file_path = options.AddOption("--db_file_path", "symbols_db.db");

  const auto app = stonks::cli::App{argc, argv, options};
  const auto injector = stonks::di::MakeInjector(
      stonks::common::CreateNetworkRestsdkInjector(),
      stonks::common::CreateSqldbSqliteInjector(),
      stonks::common::CreateLogSpdlogInjector(),

      stonks::service::CreateServerInjector<stonks::core::SymbolsDb>(
          server_options),

      stonks::di::BindValueTypeToValue(
          stonks::sqlite::FilePath{std::move(*db_file_path)}));

  app.Run([&injector]() {
    return stonks::di::CallWithInjectedArgs(
        stonks::networkx::MakeServerFor<stonks::core::ISymbolsDb>, injector);
  });
}