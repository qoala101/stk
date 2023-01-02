#include <boost/di.hpp>

#include "cli_app.h"
#include "cli_options.h"
#include "common_create_log_spdlog_injector.h"
#include "common_create_network_restsdk_injector.h"
#include "di_call_with_injected_args.h"
#include "di_make_injector.h"
#include "network_rest_server.h"
#include "networkx_make_server_for.h"
#include "service_create_server_injector.h"
#include "service_fallback.h"
#include "service_fb_traits.h"  // IWYU pragma: keep
#include "service_server_options.h"

auto main(int argc, const char *const *argv) -> int {
  auto options = stonks::cli::Options{};

  const auto server_options =
      stonks::service::ServerOptions<stonks::service::Fallback>{options};

  const auto app = stonks::cli::App{argc, argv, options};
  const auto injector = stonks::di::MakeInjector(
      stonks::common::CreateNetworkRestsdkInjector(),
      stonks::common::CreateLogSpdlogInjector(),

      stonks::service::CreateServerInjector<stonks::service::Fallback>(
          server_options));

  app.Run([&injector]() {
    return stonks::di::CallWithInjectedArgs(
        stonks::networkx::MakeServerFor<stonks::service::Fallback>, injector);
  });
}