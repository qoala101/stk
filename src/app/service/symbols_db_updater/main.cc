// clang-format off
#include "common_json_conversions.h"  // IWYU pragma: keep
// clang-format on

#include <absl/time/time.h>

#include <boost/di.hpp>
#include <cstdint>
#include <memory>
#include <variant>

#include "cli_app.h"
#include "cli_option.h"
#include "cli_options.h"
#include "common_create_log_spdlog_injector.h"
#include "common_create_network_restsdk_injector.h"
#include "core_i_symbols_db_updater.h"
#include "core_sdbu_old_prices_deleter.h"
#include "core_sdbu_symbols_info_updater.h"
#include "core_symbols_db_updater.h"
#include "di_auto_injectable.h"
#include "di_make_injector.h"
#include "networkx_make_server_for.h"
#include "service_client_options.h"
#include "service_create_client_injector.h"
#include "service_create_server_injector.h"
#include "service_sdb_traits.h"  // IWYU pragma: keep
#include "service_sdu_traits.h"  // IWYU pragma: keep
#include "service_server_options.h"
#include "service_symbols_db.h"

auto main(int argc, const char *const *argv) -> int {
  auto options = stonks::cli::Options();

  const auto server_options =
      stonks::service::ServerOptions<stonks::core::ISymbolsDbUpdater>{options};
  const auto symbols_db_client_options =
      stonks::service::ClientOptions<stonks::core::ISymbolsDb>{options};
  const auto update_symbols_info_interval =
      options.AddOption("--update_symbols_info_interval",
                        absl::ToInt64Milliseconds(absl::Hours(1)));
  const auto delete_old_prices_interval =
      options.AddOption("--delete_old_prices_interval",
                        absl::ToInt64Milliseconds(absl::Hours(1)));
  const auto keep_prices_for_duration = options.AddOption(
      "--keep_prices_for_duration", absl::ToInt64Milliseconds(absl::Hours(1)));
  const auto reattempt_interval = options.AddOption(
      "--reattempt_interval", absl::ToInt64Milliseconds(absl::Minutes(1)));

  const auto app = stonks::cli::App{argc, argv, options};
  const auto injector = stonks::di::MakeInjector(
      stonks::common::CreateNetworkRestsdkInjector(),
      stonks::common::CreateLogSpdlogInjector(),

      stonks::service::CreateServerInjector<stonks::core::SymbolsDbUpdater>(
          server_options),
      stonks::service::CreateClientInjector<stonks::service::SymbolsDb>(
          symbols_db_client_options),

      stonks::di::BindValueTypeToValue(
          absl::Milliseconds(*keep_prices_for_duration)));

  app.Run([&injector, &update_symbols_info_interval,
           &delete_old_prices_interval, &reattempt_interval]() {
    auto auto_injectable =
        stonks::di::AutoInjectable{stonks::cpp::AssumeNn(&injector)};

    return stonks::networkx::MakeServerFor<stonks::core::ISymbolsDbUpdater>(
        stonks::cpp::MakeNnSp<stonks::core::SymbolsDbUpdater>(
            stonks::core::SymbolsDbUpdater{
                absl::Milliseconds(*update_symbols_info_interval),
                auto_injectable,
                absl::Milliseconds(*delete_old_prices_interval),
                auto_injectable, absl::Milliseconds(*reattempt_interval)}),
        auto_injectable, auto_injectable);
  });
}