#include <absl/time/time.h>

#include <boost/di.hpp>
#include <cstdint>
#include <string>
#include <utility>
#include <variant>

#include "cli_app.h"
#include "cli_option.h"
#include "cli_options.h"
#include "core_symbol_prices_stream.h"
#include "core_types.h"
#include "di_bind_type_to_value.h"
#include "di_make_injector.h"
#include "service_log_spdlog_injector.h"
#include "service_network_restsdk_injector.h"
#include "service_sdb_injector.h"
#include "service_sdb_options.h"
#include "service_spsc_injector.h"
#include "service_spsc_options.h"

auto main(int argc, const char *const *argv) -> int {
  auto options = stonks::cli::Options{};

  const auto reattempt_interval = options.AddOption(
      "--reattempt_interval", absl::ToInt64Milliseconds(absl::Minutes(1)));
  const auto sdb_options = stonks::service::sdb::Options{options};
  const auto spsc_options = stonks::service::spsc::Options{options};

  const auto app = stonks::cli::App{argc, argv, options};
  const auto injector = stonks::di::MakeInjector(
      stonks::service::injectors::CreateNetworkRestsdkInjector(),
      stonks::service::injectors::CreateLogSpdlogInjector(),
      stonks::service::sdb::CreateInjector(sdb_options),
      stonks::service::spsc::CreateInjector(spsc_options),

      stonks::di::BindValueTypeToValue(
          absl::Milliseconds(*reattempt_interval)));

  app.Run([&injector]() {
    return injector.template create<stonks::core::SymbolPricesStream>();
  });
}