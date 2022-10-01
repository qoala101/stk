#include <boost/di.hpp>
#include <memory>

#include "app_log_spdlog_injector.h"
#include "app_network_restsdk_injector.h"
#include "app_sps_app.h"
#include "cli_app.h"
#include "cli_options.h"
#include "core_types.h"
#include "di_bind_type_to_value.h"
#include "di_make_injector.h"

auto main(int argc, const char *const *argv) -> int {
  stonks::cli::App{argc, argv}.Run([](const stonks::cli::Options &options) {
    const auto injector = stonks::di::MakeInjector(
        stonks::app::injectors::MakeNetworkRestsdkInjector(),
        stonks::app::injectors::MakeLogSpdlogInjector(),
        stonks::di::BindTypeToValue<stonks::core::Symbol>(
            stonks::core::Symbol{options.GetOptionOr("symbol", "btcusdt")}));

    return injector.create<stonks::app::sps::App>();
  });
}