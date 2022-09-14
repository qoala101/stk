#ifndef STONKS_SPDLOG_SPDLOG_LOGGER_H_
#define STONKS_SPDLOG_SPDLOG_LOGGER_H_

#include <string>

#include "log_i_logger.h"

namespace stonks::spdlog {
class Logger : public log::ILogger {
 public:
  /**
   * @brief Log important event.
   */
  void LogImportantEvent(std::string message) override;

  /**
   * @brief Log unexpected event.
   */
  void LogStrangeEvent(std::string message) override;

  /**
   * @brief Log error condition.
   */
  void LogErrorCondition(std::string message) override;
};
}  // namespace stonks::spdlog

#endif  // STONKS_SPDLOG_SPDLOG_LOGGER_H_
