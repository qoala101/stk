// clang-format off
#include "core_json_conversions.h"  // IWYU pragma: keep
// clang-format on

#include <fmt/core.h>

#include <boost/di.hpp>
#include <string>
#include <utility>
#include <variant>

#include "cli_app.h"
#include "cli_option.h"
#include "cli_options.h"
#include "core_i_symbols_db.h"
#include "core_symbols_db.h"
#include "di_bind_value_type_to_value.h"
#include "di_call_with_injected_args.h"
#include "di_make_injector.h"
#include "network_json_common_conversions.h"  // IWYU pragma: keep
#include "network_rest_server.h"
#include "networkx_make_server_for.h"
#include "networkx_uri.h"
#include "service_create_log_spdlog_injector.h"
#include "service_create_network_restsdk_injector.h"
#include "service_create_server_injector.h"
#include "service_create_sqldb_sqlite_injector.h"
#include "service_sdb_traits.h"  // IWYU pragma: keep
#include "service_server_options.h"
#include "sqlite_types.h"

namespace stonks::service::sdb {
void Main(int argc, const char *const *argv) {
  auto options = cli::Options{};

  const auto server_options = ServerOptions<core::ISymbolsDb>{options};
  auto db_file_path = options.AddOption("--db_file_path", "symbols_db.db");

  const auto app = cli::App{argc, argv, options};
  const auto injector = di::MakeInjector(
      CreateNetworkRestsdkInjector(), CreateSqldbSqliteInjector(),
      CreateLogSpdlogInjector(),

      CreateServerInjector<core::SymbolsDb>(server_options),

      di::BindValueTypeToValue(sqlite::FilePath{std::move(*db_file_path)}));

  app.Run([&injector]() {
    return di::CallWithInjectedArgs(networkx::MakeServerFor<core::ISymbolsDb>,
                                    injector);
  });
}
}  // namespace stonks::service::sdb

auto main(int argc, const char *const *argv) -> int {
  stonks::service::sdb::Main(argc, argv);
}