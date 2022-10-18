#ifndef STONKS_SPDLOG_SPDLOG_LOGGER_H_
#define STONKS_SPDLOG_SPDLOG_LOGGER_H_

#include <string>

#include "log_i_logger.h"

namespace stonks::spdlog {
/**
 * @copydoc log::ILogger
 */
class Logger : public log::ILogger {
 public:
  /**
   * @copydoc log::ILogger::LogEvent
   */
  void LogEvent(std::string message) override;

  /**
   * @copydoc log::ILogger::LogStrangeEvent
   */
  void LogStrangeEvent(std::string message) override;

  /**
   * @copydoc log::ILogger::LogErrorCondition
   */
  void LogErrorCondition(std::string message) override;
};
}  // namespace stonks::spdlog

#endif  // STONKS_SPDLOG_SPDLOG_LOGGER_H_
