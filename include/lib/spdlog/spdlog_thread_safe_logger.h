#ifndef VH_SPDLOG_THREAD_SAFE_LOGGER_H_
#define VH_SPDLOG_THREAD_SAFE_LOGGER_H_

#include <string>

#include "log_i_logger.h"
#include "log_types.h"

namespace vh::spdlog {
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
}  // namespace vh::spdlog

#endif  // VH_SPDLOG_THREAD_SAFE_LOGGER_H_
