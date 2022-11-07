#include <absl/time/time.h>
#include <fmt/core.h>

#include <boost/di.hpp>
#include <cstdint>
#include <memory>

#include "app_log_spdlog_injector.h"
#include "app_network_restsdk_injector.h"
#include "app_siu_app.h"
#include "app_siu_sdb_app_client.h"
#include "cli_app.h"
#include "cli_options.h"
#include "di_bind_type_to_factory_function.h"
#include "di_bind_type_to_value.h"
#include "di_make_injector.h"
#include "di_override_bindings_for_type.h"
#include "network_types.h"

auto main(int argc, const char *const *argv) -> int {
  stonks::cli::App{argc, argv}.Run([](const auto &options) {
    struct DurationFactory {
      [[nodiscard]] auto operator()(int64_t milliseconds) {
        return absl::Milliseconds(milliseconds);
      }
    };

    auto base_injector = stonks::di::MakeInjector(
        stonks::app::injectors::CreateNetworkRestsdkInjector(),
        stonks::app::injectors::CreateLogSpdlogInjector(),
        stonks::di::BindTypeToValue<int64_t>(options.GetOptionOr(
            "interval", int64_t{absl::ToInt64Milliseconds(absl::Hours(1))})),
        stonks::di::BindTypeToFactoryFunction<absl::Duration, DurationFactory,
                                              int64_t>());

    const auto injector =
        stonks::di::OverrideBindingsForType<stonks::app::siu::SdbAppClient>(
            base_injector,
            stonks::di::BindTypeToValue<stonks::network::Uri>(
                stonks::network::Uri{fmt::format(
                    "http://{}:{}", options.GetOptionOr("sdb_host", "0.0.0.0"),
                    options.GetOptionOr("sdb_port", 6506))}));

    return injector.template create<stonks::app::siu::App>();
  });
}