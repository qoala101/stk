#include <boost/di.hpp>
#include <memory>

#include "app_dt_stec_app_server.h"
#include "app_dt_stec_pds_app_client.h"
#include "app_log_spdlog_injector.h"
#include "app_network_restsdk_injector.h"
#include "app_wait_for_interrupt.h"
#include "cpp_di_bind_type_to_value.h"
#include "cpp_di_make_injector.h"
#include "cpp_di_override_bindings_for_type.h"
#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "network_types.h"
#include "not_null.hpp"

auto main(int /*unused*/, char* /*unused*/[]) -> int {
  auto default_injector = stonks::cpp::di::MakeInjector(
      stonks::app::injectors::MakeNetworkRestsdkInjector(),
      stonks::app::injectors::MakeLogSpdlogInjector(),
      stonks::cpp::di::BindTypeToValue<stonks::network::Uri>(
          stonks::network::Uri{"http://0.0.0.0:6507"}));

  const auto injector = stonks::cpp::di::OverrideBindingsForType<
      stonks::app::dt::stec::PdsAppClient>(
      default_injector, stonks::cpp::di::BindTypeToValue<stonks::network::Uri>(
                            stonks::network::Uri{"http://0.0.0.0:6506"}));

  const auto logger =
      injector.create<stonks::cpp::NnUp<stonks::log::ILogger>>();
  logger->LogImportantEvent("Started: server_to_external_client");

  const auto server = injector.create<stonks::app::dt::stec::AppServer>();

  stonks::app::WaitForInterrupt();
  logger->LogImportantEvent("Ended: server_to_external_client");
}