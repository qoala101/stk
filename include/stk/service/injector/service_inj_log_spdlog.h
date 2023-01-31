#ifndef VH_STK_SERVICE_INJ_LOG_SPDLOG_H_
#define VH_STK_SERVICE_INJ_LOG_SPDLOG_H_

#include "di_bind_interface_to_implementation.h"
#include "di_make_injector.h"
#include "log_i_logger.h"
#include "spdlog_thread_safe_logger.h"

namespace vh::stk::service::inj {
inline auto CreateLogSpdlogInjector [[nodiscard]] () {
  return di::MakeInjector(
      di::BindInterfaceToImplementation<log::ILogger, spdlog::ThreadSafeLogger>());
}
}  // namespace vh::stk::service

#endif  // VH_STK_SERVICE_INJ_LOG_SPDLOG_H_
