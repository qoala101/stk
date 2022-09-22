#include <CLI/App.hpp>
#include <CLI/Config.hpp>     // IWYU pragma: keep
#include <CLI/Formatter.hpp>  // IWYU pragma: keep
#include <boost/di.hpp>
#include <memory>
#include <string>
#include <utility>

#include "app_dt_stec_app_server.h"
#include "app_dt_stec_pds_app_client.h"
#include "app_log_spdlog_injector.h"
#include "app_network_restsdk_injector.h"
#include "app_wait_for_interrupt.h"
#include "cpp_di_bind_type_to_value.h"
#include "cpp_di_make_injector.h"
#include "cpp_di_override_bindings_for_type.h"
#include "cpp_format.h"
#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "network_types.h"
#include "not_null.hpp"

auto main(int argc, char* argv[]) -> int {
  auto app = CLI::App{};

  auto port = 6507;
  app.add_option("--port", port);

  auto pds_host = std::string{"0.0.0.0"};
  app.add_option("--pds_host", pds_host);

  auto pds_port = 6506;
  app.add_option("--pds_port", pds_port);

  app.parse(argc, argv);

  const auto app_name = app.get_name();

  auto default_injector = stonks::cpp::di::MakeInjector(
      stonks::app::injectors::MakeNetworkRestsdkInjector(),
      stonks::app::injectors::MakeLogSpdlogInjector(),
      stonks::cpp::di::BindTypeToValue<stonks::network::Uri>(
          stonks::network::Uri{
              stonks::cpp::Format("http://0.0.0.0:{}", port)}));

  const auto injector = stonks::cpp::di::OverrideBindingsForType<
      stonks::app::dt::stec::PdsAppClient>(
      default_injector, stonks::cpp::di::BindTypeToValue<stonks::network::Uri>(
                            stonks::network::Uri{stonks::cpp::Format(
                                "http://{}:{}", pds_host, pds_port)}));

  const auto logger =
      injector.create<stonks::cpp::NnUp<stonks::log::ILogger>>();
  logger->LogImportantEvent(stonks::cpp::Format("Started: {}", app_name));

  const auto server = injector.create<stonks::app::dt::stec::AppServer>();

  stonks::app::WaitForInterrupt();
  logger->LogImportantEvent(stonks::cpp::Format("Ended: {}", app_name));
}