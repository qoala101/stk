#include "spdlog_logger.h"

#include <spdlog/spdlog.h>

namespace stonks::spdlog {
void Logger::LogEvent(std::string message) { ::spdlog::info(message); }

void Logger::LogStrangeEvent(std::string message) { ::spdlog::warn(message); }

void Logger::LogErrorCondition(std::string message) {
  ::spdlog::error(message);
}
}  // namespace stonks::spdlog