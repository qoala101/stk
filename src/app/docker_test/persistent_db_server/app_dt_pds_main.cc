#include <CLI/App.hpp>
#include <CLI/Config.hpp>     // IWYU pragma: keep
#include <CLI/Formatter.hpp>  // IWYU pragma: keep
#include <boost/di.hpp>
#include <memory>
#include <utility>

#include "app_dt_pds_app_server.h"
#include "app_log_spdlog_injector.h"
#include "app_network_restsdk_injector.h"
#include "app_sqldb_sqlite_injector.h"
#include "app_wait_for_interrupt.h"
#include "di_bind_type_to_value.h"
#include "di_make_injector.h"
#include "cpp_format.h"
#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "network_types.h"
#include "not_null.hpp"
#include "sqlite_types.h"

auto main(int argc, char* argv[]) -> int {
  auto app = CLI::App{};

  auto port = 6506;
  app.add_option("--port", port);

  app.parse(argc, argv);

  const auto app_name = app.get_name();

  const auto injector = stonks::di::MakeInjector(
      stonks::app::injectors::MakeNetworkRestsdkInjector(),
      stonks::app::injectors::MakeSqldbSqliteInjector(),
      stonks::app::injectors::MakeLogSpdlogInjector(),
      stonks::di::BindTypeToValue<stonks::network::Uri>(
          stonks::network::Uri{stonks::cpp::Format("http://0.0.0.0:{}", port)}),
      stonks::di::BindTypeToValue<stonks::sqlite::FilePath>(
          stonks::sqlite::FilePath{"persistent_db_server.db"}));

  const auto logger =
      injector.create<stonks::cpp::NnUp<stonks::log::ILogger>>();
  logger->LogImportantEvent(stonks::cpp::Format("Started: {}", app_name));

  const auto server = injector.create<stonks::app::dt::pds::AppServer>();

  stonks::app::WaitForInterrupt();
  logger->LogImportantEvent(stonks::cpp::Format("Ended: {}", app_name));
}