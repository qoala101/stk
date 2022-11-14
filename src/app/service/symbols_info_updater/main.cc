#include <absl/time/time.h>
#include <fmt/core.h>

#include <cstdint>

#include "cli_app.h"
#include "core_i_symbols_db.h"
#include "core_symbols_info_updater.h"
#include "di_bind_interface_to_implementation.h"
#include "di_bind_type_to_factory_function.h"
#include "di_bind_type_to_value.h"
#include "di_make_injector.h"
#include "di_override_bindings_for_type.h"
#include "network_types.h"
#include "service_log_spdlog_injector.h"
#include "service_network_restsdk_injector.h"
#include "service_symbols_db.h"

auto main(int argc, const char *const *argv) -> int {
  stonks::cli::App{argc, argv}.Run([](const auto &options) {
    struct DurationFactory {
      auto operator() [[nodiscard]] (int64_t milliseconds) {
        return absl::Milliseconds(milliseconds);
      }
    };

    auto base_injector = stonks::di::MakeInjector(
        stonks::service::injectors::CreateNetworkRestsdkInjector(),
        stonks::service::injectors::CreateLogSpdlogInjector(),
        stonks::di::BindTypeToValue<int64_t>(options.GetOptionOr(
            "interval", int64_t{absl::ToInt64Milliseconds(absl::Hours(1))})),
        stonks::di::BindTypeToFactoryFunction<absl::Duration, DurationFactory,
                                              int64_t>(),
        stonks::di::BindInterfaceToImplementation<
            stonks::core::ISymbolsDb,
            stonks::service::Connection<stonks::core::ISymbolsDb>>());

    const auto injector = stonks::di::OverrideBindingsForType<
        stonks::service::Connection<stonks::core::ISymbolsDb>>(
        base_injector,
        stonks::di::BindTypeToValue<stonks::network::Uri>(
            stonks::network::Uri{fmt::format(
                "http://{}:{}", options.GetOptionOr("sdb_host", "0.0.0.0"),
                options.GetOptionOr("sdb_port", 6506))}));

    return injector.template create<stonks::core::SymbolsInfoUpdater>();
  });
}