#include "log_i_logger.h"

#include <utility>

namespace stonks::log {
void ILogger::LogImportantEvent(std::string message) {
  LogEvent(std::move(message));
}

void ILogger::LogStrangeEvent(std::string message) {
  LogEvent(std::move(message));
}

void ILogger::LogErrorCondition(std::string message) {
  LogEvent(std::move(message));
}
}  // namespace stonks::log