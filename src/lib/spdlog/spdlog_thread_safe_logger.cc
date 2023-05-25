/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "spdlog_thread_safe_logger.h"

#include <spdlog/common.h>
#include <spdlog/spdlog.h>

namespace vh::spdlog {
namespace {
auto ToNativeLevel(log::Level level) {
  switch (level) {
    case log::Level::Info:
      return ::spdlog::level::info;
    case log::Level::Warning:
      return ::spdlog::level::warn;
    case log::Level::Error:
      return ::spdlog::level::err;
  }
}
}  // namespace

void ThreadSafeLogger::Log(log::Level level, std::string message) {
  ::spdlog::log(ToNativeLevel(level), message);
}
}  // namespace vh::spdlog