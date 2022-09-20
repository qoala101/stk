#include <boost/di.hpp>
#include <cstdio>
#include <memory>
#include <tuple>

#include "app_dt_pds_app_server.h"
#include "app_log_spdlog_injector.h"
#include "app_network_restsdk_injector.h"
#include "app_sqldb_sqlite_injector.h"
#include "app_wait_for_interrupt.h"
#include "cpp_di_bind_type_to_value.h"
#include "cpp_di_make_injector.h"
#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "network_types.h"
#include "not_null.hpp"
#include "sqlite_types.h"

auto main(int /*unused*/, char* /*unused*/[]) -> int {
  const auto injector = stonks::cpp::di::MakeInjector(
      stonks::app::injectors::MakeNetworkRestsdkInjector(),
      stonks::app::injectors::MakeSqldbSqliteInjector(),
      stonks::app::injectors::MakeLogSpdlogInjector(),
      stonks::cpp::di::BindTypeToValue<stonks::network::Uri>(
          stonks::network::Uri{"http://0.0.0.0:6506"}),
      stonks::cpp::di::BindTypeToValue<stonks::sqlite::FilePath>(
          stonks::sqlite::FilePath{"persistent_db_server.db"}));

  const auto logger =
      injector.create<stonks::cpp::NnUp<stonks::log::ILogger>>();
  logger->LogImportantEvent("Started: persistent_db_server");

  const auto server = injector.create<stonks::app::dt::pds::AppServer>();

  stonks::app::WaitForInterrupt();
  logger->LogImportantEvent("Ended: persistent_db_server");
}