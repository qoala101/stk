// clang-format off
#include "service_json_conversions.h"  // IWYU pragma: keep
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
#include "di_bind_interface_to_implementation.h"
#include "di_bind_value_type_to_value.h"
#include "di_call_with_injected_args.h"
#include "di_make_injector.h"
#include "network_json_common_conversions.h"  // IWYU pragma: keep
#include "network_rest_server.h"
#include "networkx_make_server_for.h"
#include "networkx_uri.h"
#include "service_log_spdlog_injector.h"
#include "service_network_restsdk_injector.h"
#include "service_sdb_traits.h"  // IWYU pragma: keep
#include "service_sqldb_sqlite_injector.h"
#include "sqlite_types.h"

auto main(int argc, const char *const *argv) -> int {
  auto options = stonks::cli::Options{};

  const auto port = options.AddOption("--port", 30001);
  auto db_file_path = options.AddOption("--db_file_path", "symbols_db.db");

  const auto app = stonks::cli::App{argc, argv, options};
  const auto injector = stonks::di::MakeInjector(
      stonks::service::injectors::CreateNetworkRestsdkInjector(),
      stonks::service::injectors::CreateSqldbSqliteInjector(),
      stonks::service::injectors::CreateLogSpdlogInjector(),

      stonks::di::BindValueTypeToValue(
          stonks::sqlite::FilePath{std::move(*db_file_path)}),
      stonks::di::BindValueTypeToValue(
          stonks::networkx::Uri<stonks::core::ISymbolsDb>{
              fmt::format("http://0.0.0.0:{}", *port)}),
      stonks::di::BindInterfaceToImplementation<stonks::core::ISymbolsDb,
                                                stonks::core::SymbolsDb>());

  app.Run([&injector]() {
    return stonks::di::CallWithInjectedArgs(
        stonks::networkx::MakeServerFor<stonks::core::ISymbolsDb>, injector);
  });
}