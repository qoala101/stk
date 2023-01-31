#ifndef STONKS_LOG_LOG_LOGGER_H_
#define STONKS_LOG_LOG_LOGGER_H_

#include <string>

#include "log_types.h"

namespace vh::log {
// NOLINTNEXTLINE(*-special-member-functions)
class ILogger {
 public:
  virtual ~ILogger() = default;

  /**
   * @brief Logs message with specified level.
   */
  virtual void Log(Level level, std::string message) = 0;

  /**
   * @brief Logs important event.
   */
  void LogImportantEvent(std::string message);

  /**
   * @brief Logs unexpected event.
   */
  void LogStrangeEvent(std::string message);

  /**
   * @brief Logs error condition.
   */
  void LogErrorCondition(std::string message);
};
}  // namespace vh::log

#endif  // STONKS_LOG_LOG_LOGGER_H_
