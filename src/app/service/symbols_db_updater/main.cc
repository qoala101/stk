// clang-format off
#include "core_json_conversions.h"  // IWYU pragma: keep
// clang-format on

#include <absl/time/time.h>

#include <boost/di.hpp>
#include <cstdint>
#include <memory>
#include <variant>

#include "cli_app.h"
#include "cli_option.h"
#include "cli_options.h"
#include "core_symbols_db_updater.h"
#include "cpp_share.h"
#include "di_auto_injectable.h"
#include "di_make_injector.h"
#include "service_client_options.h"
#include "service_create_client_injector.h"
#include "service_create_log_spdlog_injector.h"
#include "service_create_network_restsdk_injector.h"
#include "service_sdb_traits.h"  // IWYU pragma: keep
#include "service_symbols_db.h"

namespace stonks::service::sdbu {
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
  const auto delete_old_prices_interval =
      options.AddOption("--delete_old_prices_interval",
                        absl::ToInt64Milliseconds(absl::Hours(1)));
  const auto keep_prices_for_duration = options.AddOption(
      "--keep_prices_for_duration", absl::ToInt64Milliseconds(absl::Hours(1)));
  const auto reattempt_interval = options.AddOption(
      "--reattempt_interval", absl::ToInt64Milliseconds(absl::Minutes(1)));

  const auto app = cli::App{argc, argv, options};
  const auto injector = cpp::Share(di::MakeInjector(
      CreateNetworkRestsdkInjector(), CreateLogSpdlogInjector(),

      CreateClientInjector<SymbolsDb>(symbols_db_client_options),

      di::BindValueTypeToValue(absl::Milliseconds(*keep_prices_for_duration))));

  app.Run([&injector, &update_symbols_info_interval,
           &check_if_update_required_interval, &delete_old_prices_interval,
           &keep_prices_for_duration, &reattempt_interval]() {
    auto auto_injectable = di::AutoInjectable{injector};

    return core::SymbolsDbUpdater{
        {.symbols_db = auto_injectable,
         .binance_api = auto_injectable,
         .update_symbols_info_interval =
             absl::Milliseconds(*update_symbols_info_interval),
         .check_if_update_required_interval =
             absl::Milliseconds(*check_if_update_required_interval),
         .delete_old_prices_interval =
             absl::Milliseconds(*delete_old_prices_interval),
         .keep_prices_for_duration =
             absl::Milliseconds(*keep_prices_for_duration),
         .reattempt_interval = absl::Milliseconds(*reattempt_interval)}};
  });
}
}  // namespace stonks::service::sdbu

auto main(int argc, const char *const *argv) -> int {
  stonks::service::sdbu::Main(argc, argv);
}