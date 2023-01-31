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
#include "core_old_symbol_prices_deleter.h"
#include "cpp_share.h"
#include "di_auto_injectable.h"
#include "di_make_injector.h"
#include "service_client_options.h"
#include "service_inj_client_server.h"
#include "service_inj_log_spdlog.h"
#include "service_inj_network_restsdk.h"
#include "service_sdb_traits.h"  // IWYU pragma: keep
#include "service_symbols_db.h"

namespace vh::stk::service::sdbu {
void Main(int argc, const char *const *argv) {
  auto options = cli::Options();

  const auto symbols_db_client_options =
      ClientOptions<core::ISymbolsDb>{options};
  const auto delete_old_prices_interval =
      options.AddOption("--delete_old_prices_interval",
                        absl::ToInt64Milliseconds(absl::Hours(1)));
  const auto keep_prices_for_duration = options.AddOption(
      "--keep_prices_for_duration", absl::ToInt64Milliseconds(absl::Hours(1)));
  const auto reattempt_interval = options.AddOption(
      "--reattempt_interval", absl::ToInt64Milliseconds(absl::Minutes(1)));

  const auto app = cli::App{argc, argv, options};
  const auto injector = cpp::Share(di::MakeInjector(
      inj::CreateNetworkRestsdkInjector(), inj::CreateLogSpdlogInjector(),
      inj::CreateClientInjector<SymbolsDb>(symbols_db_client_options)));

  app.Run([&injector, &delete_old_prices_interval, &keep_prices_for_duration,
           &reattempt_interval]() {
    auto auto_injectable = di::AutoInjectable{injector};

    return core::OldSymbolPricesDeleter{
        {.symbols_db = auto_injectable,
         .delete_old_prices_interval =
             absl::Milliseconds(*delete_old_prices_interval),
         .keep_prices_for_duration =
             absl::Milliseconds(*keep_prices_for_duration),
         .reattempt_interval = absl::Milliseconds(*reattempt_interval)}};
  });
}
}  // namespace vh::stk::service::sdbu

auto main(int argc, const char *const *argv) -> int {
  vh::stk::service::sdbu::Main(argc, argv);
}