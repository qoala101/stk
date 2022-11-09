#ifndef STONKS_APP_APP_LOG_SPDLOG_INJECTOR_H_
#define STONKS_APP_APP_LOG_SPDLOG_INJECTOR_H_

#include "di_bind_interface_to_implementation.h"
#include "di_make_injector.h"
#include "log_i_logger.h"
#include "spdlog_logger.h"

namespace stonks::app::injectors {
inline auto CreateLogSpdlogInjector [[nodiscard]] () {
  return di::MakeInjector(
      di::BindInterfaceToImplementation<log::ILogger, spdlog::Logger>());
}
}  // namespace stonks::app::injectors

#endif  // STONKS_APP_APP_LOG_SPDLOG_INJECTOR_H_
