#include "spdlog_thread_safe_logger.h"

#include <spdlog/common.h>
#include <spdlog/spdlog.h>

namespace stonks::spdlog {
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
}  // namespace stonks::spdlog