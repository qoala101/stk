/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_LOG_I_LOGGER_H_
#define VH_LOG_I_LOGGER_H_

#include <string>

#include "log_types.h"

namespace vh::log {
// NOLINTNEXTLINE(*-special-member-functions)
class ILogger {
 public:
  virtual ~ILogger() = default;

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

 private:
  /**
   * @brief Logs message with specified level.
   */
  virtual void Log(Level level, std::string message) = 0;
};
}  // namespace vh::log

#endif  // VH_LOG_I_LOGGER_H_
