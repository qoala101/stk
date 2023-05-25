/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

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
 private:
  /**
   * @copydoc log::ILogger::Log
   */
  void Log(log::Level level, std::string message) override;
};
}  // namespace vh::spdlog

#endif  // VH_SPDLOG_THREAD_SAFE_LOGGER_H_
