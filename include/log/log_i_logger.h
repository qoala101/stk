#ifndef STONKS_LOG_LOG_LOGGER_H_
#define STONKS_LOG_LOG_LOGGER_H_

#include <string>

namespace stonks::log {
// NOLINTNEXTLINE(*-special-member-functions)
class ILogger {
 public:
  virtual ~ILogger() = default;

  /**
   * @brief Log important event.
   */
  virtual void LogImportantEvent(std::string message) = 0;

  /**
   * @brief Log unexpected event.
   */
  virtual void LogStrangeEvent(std::string message) = 0;

  /**
   * @brief Log error condition.
   */
  virtual void LogErrorCondition(std::string message) = 0;
};
}  // namespace stonks::log

#endif  // STONKS_LOG_LOG_LOGGER_H_
