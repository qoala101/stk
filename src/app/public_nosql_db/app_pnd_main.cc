#include <boost/di.hpp>
#include <cstdio>
#include <memory>
#include <tuple>

#include "app_log_spdlog_injector.h"
#include "app_network_restsdk_injector.h"
#include "app_nosqldb_aws_injector.h"
#include "app_pnd_items_interface_server.h"
#include "app_wait_for_interrupt.h"
#include "cpp_di_bind_type_to_value.h"
#include "cpp_di_make_injector.h"
#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "network_types.h"
#include "not_null.hpp"

auto main(int /*unused*/, char* /*unused*/[]) -> int {
  const auto injector = stonks::cpp::di::MakeInjector(
      stonks::app::injectors::MakeNetworkRestsdkInjector(),
      stonks::app::injectors::MakeNosqldbAwsInjector(),
      stonks::app::injectors::MakeLogSpdlogInjector(),
      stonks::cpp::di::BindTypeToValue<stonks::network::Uri>(
          stonks::network::Uri{"http://0.0.0.0:6506/Entity"}));

  const auto logger =
      injector.create<stonks::cpp::NnUp<stonks::log::ILogger>>();
  logger->LogImportantEvent("Started: public_nosql_db");

  const auto server = injector.create<stonks::app::pnd::ItemsInterfaceServer>();

  stonks::app::WaitForInterrupt();
  logger->LogImportantEvent("Ended: public_nosql_db");
}