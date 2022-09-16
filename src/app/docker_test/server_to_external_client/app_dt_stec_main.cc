#include <boost/di.hpp>
#include <cstdio>
#include <memory>
#include <tuple>

#include "app_dt_stec_app.h"
#include "app_dt_stec_app_server.h"
#include "app_log_spdlog_injector.h"
#include "app_network_restsdk_injector.h"
#include "cpp_di_enable_not_null.h"
#include "cpp_not_null.h"
#include "cpp_smart_pointers.h"
#include "log_i_logger.h"
#include "network_types.h"
#include "not_null.hpp"

auto main(int /*unused*/, char* /*unused*/[]) -> int {
  // clang-format off
  const auto injector = make_injector(
    stonks::app::injectors::MakeNetworkRestsdkInjector(),
    stonks::app::injectors::MakeLogSpdlogInjector(),

    stonks::cpp::di::EnableNn<stonks::cpp::Sp<stonks::app::dt::App>>(),
    boost::di::bind<stonks::network::Uri>.to(stonks::network::Uri{"http://localhost:6506"})
  );
  // clang-format on

  const auto logger =
      injector.create<stonks::cpp::NnUp<stonks::log::ILogger>>();
  logger->LogImportantEvent("Started: server_to_external_client");

  const auto server = injector.create<stonks::app::dt::AppServer>();

  std::ignore = getchar();
  logger->LogImportantEvent("Ended: server_to_external_client");
}