#ifndef STONKS_SPDLOG_SPDLOG_LOGGER_H_
#define STONKS_SPDLOG_SPDLOG_LOGGER_H_

#include <string>

#include "log_i_logger.h"
#include "log_types.h"

namespace stonks::spdlog {
/**
 * @copydoc log::ILogger
 */
class Logger : public log::ILogger {
 public:
  /**
   * @copydoc log::ILogger::Log
   */
  void Log(log::Level level, std::string message) override;
};
}  // namespace stonks::spdlog

#endif  // STONKS_SPDLOG_SPDLOG_LOGGER_H_
