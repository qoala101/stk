// clang-format off
#include "core_json_conversions.h"  // IWYU pragma: keep
// clang-format on

#include <absl/container/flat_hash_map.h>

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
#include "service_inj_client_server.h"
#include "service_inj_log_spdlog.h"
#include "service_inj_network_restsdk.h"
#include "service_inj_sqldb_sqlite.h"
#include "service_sdb_traits.h"  // IWYU pragma: keep
#include "service_server_options.h"
#include "sqlite_types.h"

namespace vh::stk::service::sdb {
void Main(int argc, const char *const *argv) {
  auto options = cli::Options{};

  const auto server_options = ServerOptions<core::ISymbolsDb>{options};
  auto db_file_path = options.AddOption("--db_file_path", "symbols_db.db");

  const auto app = cli::App{argc, argv, options};
  const auto injector = di::MakeInjector(
      inj::CreateNetworkRestsdkInjector(), inj::CreateSqldbSqliteInjector(),
      inj::CreateLogSpdlogInjector(),
      inj::CreateServerInjector<core::SymbolsDb>(server_options),
      di::BindValueTypeToValue(sqlite::FilePath{std::move(*db_file_path)}));

  app.Run([&injector]() {
    return di::CallWithInjectedArgs(networkx::MakeServerFor<core::ISymbolsDb>,
                                    injector);
  });
}
}  // namespace vh::stk::service::sdb

auto main(int argc, const char *const *argv) -> int {
  vh::stk::service::sdb::Main(argc, argv);
}