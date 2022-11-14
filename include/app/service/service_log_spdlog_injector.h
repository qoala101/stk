#ifndef STONKS_APP_SERVICE_SERVICE_LOG_SPDLOG_INJECTOR_H_
#define STONKS_APP_SERVICE_SERVICE_LOG_SPDLOG_INJECTOR_H_

#include "di_bind_interface_to_implementation.h"
#include "di_make_injector.h"
#include "log_i_logger.h"
#include "spdlog_logger.h"

namespace stonks::service::injectors {
inline auto CreateLogSpdlogInjector [[nodiscard]] () {
  return di::MakeInjector(
      di::BindInterfaceToImplementation<log::ILogger, spdlog::Logger>());
}
}  // namespace stonks::service::injectors

#endif  // STONKS_APP_SERVICE_SERVICE_LOG_SPDLOG_INJECTOR_H_
