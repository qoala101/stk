// clang-format off
#include "service_blsh_json_conversions.h"  // IWYU pragma: keep
// clang-format on

#include <fmt/core.h>

#include <boost/di.hpp>
#include <string>
#include <variant>

#include "cli_app.h"
#include "cli_option.h"
#include "cli_options.h"
#include "core_buy_low_sell_high.h"
#include "cpp_not_null.h"
#include "di_enable_nn_pointers.h"
#include "di_make_injector.h"
#include "network_i_rest_request_receiver.h"
#include "network_json_common_conversions.h"  // IWYU pragma: keep
#include "network_rest_server_builder.h"
#include "network_types.h"
#include "networkx_make_server_for.h"
#include "service_blsh_traits.h"  // IWYU pragma: keep
#include "service_log_spdlog_injector.h"
#include "service_network_restsdk_injector.h"
#include "service_sdb_injector.h"
#include "service_sdb_options.h"

auto main(int argc, const char *const *argv) -> int {
  auto options = stonks::cli::Options();

  const auto port = options.AddOption("--port", 30000);
  const auto sdb_options = stonks::service::sdb::Options{options};

  const auto app = stonks::cli::App{argc, argv, options};
  const auto injector = stonks::di::MakeInjector(
      stonks::service::injectors::CreateNetworkRestsdkInjector(),
      stonks::service::injectors::CreateLogSpdlogInjector(),
      stonks::service::sdb::CreateInjector(sdb_options),

      stonks::di::EnableNnPointers<stonks::core::BuyLowSellHigh>());

  app.Run([&injector, &port]() {
    return stonks::networkx::MakeServerFor(
        injector
            .template create<stonks::cpp::NnSp<stonks::core::BuyLowSellHigh>>(),
        stonks::network::RestServerBuilder{
            stonks::network::Uri{fmt::format("http://0.0.0.0:{}", *port)},
            injector.template create<
                stonks::cpp::NnUp<stonks::network::IRestRequestReceiver>>()});
  });
}