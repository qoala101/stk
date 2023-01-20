#ifndef STONKS_SPDLOG_SPDLOG_LOGGER_H_
#define STONKS_SPDLOG_SPDLOG_LOGGER_H_

#include <string>

#include "log_i_logger.h"
#include "log_types.h"

namespace stonks::spdlog {
/**
 * @brief Logger which can be used in multiple threads.
 */
class ThreadSafeLogger : public log::ILogger {
 public:
  /**
   * @copydoc log::ILogger::Log
   */
  void Log(log::Level level, std::string message) override;
};
}  // namespace stonks::spdlog

#endif  // STONKS_SPDLOG_SPDLOG_LOGGER_H_
