#ifndef STONKS_APP_APP_LOG_SPDLOG_INJECTOR_H_
#define STONKS_APP_APP_LOG_SPDLOG_INJECTOR_H_

#include <boost/di.hpp>

#include "cpp_di_enable_not_null.h"
#include "cpp_smart_pointers.h"
#include "log_i_logger.h"
#include "spdlog_logger.h"

namespace stonks::app::injectors {
[[nodiscard]] inline auto MakeLogSpdlogInjector() {
  // clang-format off
  return make_injector(
    boost::di::bind<log::ILogger>().to<spdlog::Logger>(),
    cpp::di::EnableNn<cpp::Up<log::ILogger>>(),
    cpp::di::EnableNn<cpp::Sp<log::ILogger>>()
  );
  // clang-format on
}
}  // namespace stonks::app::injectors

#endif  // STONKS_APP_APP_LOG_SPDLOG_INJECTOR_H_
