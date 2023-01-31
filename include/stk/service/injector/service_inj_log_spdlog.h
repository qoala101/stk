#ifndef STONKS_APP_COMMON_COMMON_CREATE_LOG_SPDLOG_INJECTOR_H_
#define STONKS_APP_COMMON_COMMON_CREATE_LOG_SPDLOG_INJECTOR_H_

#include "di_bind_interface_to_implementation.h"
#include "di_make_injector.h"
#include "log_i_logger.h"
#include "spdlog_thread_safe_logger.h"

namespace stonks::service::inj {
inline auto CreateLogSpdlogInjector [[nodiscard]] () {
  return di::MakeInjector(
      di::BindInterfaceToImplementation<log::ILogger, spdlog::ThreadSafeLogger>());
}
}  // namespace stonks::service

#endif  // STONKS_APP_COMMON_COMMON_CREATE_LOG_SPDLOG_INJECTOR_H_
