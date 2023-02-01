// clang-format off
#include "service_json_conversions.h"  // IWYU pragma: keep
// clang-format on

#include <absl/time/time.h>

#include <boost/di.hpp>
#include <cstdint>
#include <memory>
#include <not_null.hpp>
#include <type_traits>
#include <utility>
#include <variant>

#include "cli_app.h"
#include "cli_option.h"
#include "cli_options.h"
#include "core_i_symbols_db.h"
#include "core_symbols_info_updater.h"
#include "cpp_share.h"
#include "di_auto_injectable.h"
#include "di_make_injector.h"
#include "service_client_options.h"
#include "service_inj_log_spdlog.h"
#include "service_inj_network_restsdk.h"
#include "service_sdb_client_injector.h"
#include "service_sdb_traits.h"  // IWYU pragma: keep

namespace vh::stk::service::sdbu {
void Main(int argc, const char *const *argv) {
  auto options = cli::Options();

  const auto symbols_db_client_options =
      ClientOptions<core::ISymbolsDb>{options};
  const auto update_symbols_info_interval =
      options.AddOption("--update_symbols_info_interval",
                        absl::ToInt64Milliseconds(absl::Hours(1)));
  const auto check_if_update_required_interval =
      options.AddOption("--check_if_update_required_interval",
                        absl::ToInt64Milliseconds(absl::Minutes(1)));
  const auto reattempt_interval = options.AddOption(
      "--reattempt_interval", absl::ToInt64Milliseconds(absl::Minutes(1)));

  const auto app = cli::App{argc, argv, options};
  const auto injector = cpp::Share(di::MakeInjector(
      inj::CreateNetworkRestsdkInjector(), inj::CreateLogSpdlogInjector(),
      sdb::CreateSymbolsDbClientInjector(symbols_db_client_options)));

  app.Run([&injector, &update_symbols_info_interval,
           &check_if_update_required_interval, &reattempt_interval]() {
    auto auto_injectable = di::AutoInjectable{injector};

    return core::SymbolsInfoUpdater{
        {.symbols_db = auto_injectable,
         .binance_api = auto_injectable,
         .update_symbols_info_interval =
             absl::Milliseconds(*update_symbols_info_interval),
         .check_if_update_required_interval =
             absl::Milliseconds(*check_if_update_required_interval),
         .reattempt_interval = absl::Milliseconds(*reattempt_interval)}};
  });
}
}  // namespace vh::stk::service::sdbu

auto main(int argc, const char *const *argv) -> int {
  vh::stk::service::sdbu::Main(argc, argv);
}