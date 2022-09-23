#include <boost/di.hpp>
#include <memory>

#include "app_log_spdlog_injector.h"
#include "app_network_restsdk_injector.h"
#include "app_nosqldb_aws_injector.h"
#include "app_pnd_items_interface_server.h"
#include "cli_app.h"
#include "di_bind_type_to_value.h"
#include "di_make_injector.h"
#include "network_types.h"

auto main(int argc, const char* const* argv) -> int {
  stonks::cli::App{argc, argv}.Run([]() {
    const auto injector = stonks::di::MakeInjector(
        stonks::app::injectors::MakeNetworkRestsdkInjector(),
        stonks::app::injectors::MakeNosqldbAwsInjector(),
        stonks::app::injectors::MakeLogSpdlogInjector(),
        stonks::di::BindTypeToValue<stonks::network::Uri>(
            stonks::network::Uri{"http://0.0.0.0:6506/Entity"}));

    return injector.create<stonks::app::pnd::ItemsInterfaceServer>();
  });
}