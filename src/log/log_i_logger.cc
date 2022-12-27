#include "log_i_logger.h"

#include <utility>

#include "log_types.h"

namespace stonks::log {
void ILogger::LogImportantEvent(std::string message) {
  Log(Level::Info, std::move(message));
}

void ILogger::LogStrangeEvent(std::string message) {
  Log(Level::Warning, std::move(message));
}

void ILogger::LogErrorCondition(std::string message) {
  Log(Level::Error, std::move(message));
}
}  // namespace stonks::log