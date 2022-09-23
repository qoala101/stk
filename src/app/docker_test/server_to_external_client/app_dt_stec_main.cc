#include <fmt/core.h>

#include <boost/di.hpp>
#include <memory>

#include "app_dt_stec_app_server.h"
#include "app_dt_stec_pds_app_client.h"
#include "app_log_spdlog_injector.h"
#include "app_network_restsdk_injector.h"
#include "cli_app.h"
#include "cli_options.h"
#include "di_bind_type_to_value.h"
#include "di_make_injector.h"
#include "di_override_bindings_for_type.h"
#include "network_types.h"

auto main(int argc, const char *const *argv) -> int {
  stonks::cli::App{argc, argv}.Run([](const stonks::cli::Options &options) {
    auto default_injector = stonks::di::MakeInjector(
        stonks::app::injectors::MakeNetworkRestsdkInjector(),
        stonks::app::injectors::MakeLogSpdlogInjector(),
        stonks::di::BindTypeToValue<stonks::network::Uri>(
            stonks::network::Uri{fmt::format(
                "http://0.0.0.0:{}", options.GetOptionOr("port", 6507))}));

    const auto injector = stonks::di::OverrideBindingsForType<
        stonks::app::dt::stec::PdsAppClient>(
        default_injector,
        stonks::di::BindTypeToValue<stonks::network::Uri>(
            stonks::network::Uri{fmt::format(
                "http://{}:{}", options.GetOptionOr("pds_host", "0.0.0.0"),
                options.GetOptionOr("pds_port", 6506))}));

    return injector.create<stonks::app::dt::stec::AppServer>();
  });
}