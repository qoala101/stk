/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

// clang-format off
#include "service_json_conversions.h"  // IWYU pragma: keep
// clang-format on

#include <absl/container/flat_hash_map.h>

#include <boost/di.hpp>
#include <memory>
#include <not_null.hpp>
#include <type_traits>
#include <utility>

#include "cli_app.h"
#include "cli_options.h"
#include "core_i_symbols_db.h"
#include "core_symbol_prices_filter.h"
#include "cpp_not_null.h"
#include "cpp_share.h"
#include "di_auto_injectable.h"
#include "di_enable_nn_pointers.h"
#include "di_make_injector.h"
#include "network_json_common_conversions.h"
#include "networkx_make_server_for.h"
#include "service_client_options.h"
#include "service_inj_log_spdlog.h"
#include "service_inj_network_restsdk.h"
#include "service_sdb_client_injector.h"
#include "service_sdb_traits.h"  // IWYU pragma: keep
#include "service_server_options.h"

namespace vh::stk::service::spf {
void Main(int argc, const char *const *argv) {
  auto options = cli::Options{};

  const auto server_options = ServerOptions<core::ISymbolsDb>{options};
  const auto symbols_db_client_options =
      ClientOptions<core::ISymbolsDb>{options};

  const auto app = cli::App{argc, argv, options};
  const auto injector = cpp::Share(di::MakeInjector(
      inj::CreateNetworkRestsdkInjector(), inj::CreateLogSpdlogInjector(),
      sdb::CreateSymbolsDbClientInjector(symbols_db_client_options),
      di::EnableNnPointers<core::SymbolPricesFilter>()));

  app.Run([&injector, &server_options]() {
    auto auto_injectable = di::AutoInjectable(injector);

    return networkx::MakeServerFor<core::ISymbolsDb>(
        injector->create<cpp::NnUp<core::SymbolPricesFilter>>(),
        server_options.GetUri(), auto_injectable);
  });
}
}  // namespace vh::stk::service::spf

auto main(int argc, const char *const *argv) -> int {
  vh::stk::service::spf::Main(argc, argv);
}