#ifndef VH_LOG_REDUCED_LOGGER_H_
#define VH_LOG_REDUCED_LOGGER_H_

#include <absl/time/time.h>

#include <string>

#include "cpp_not_null.h"
#include "cpp_timer.h"
#include "log_i_logger.h"
#include "log_rl_log_stats.h"
#include "log_types.h"

namespace vh::log {
/**
 * @brief Logger wrapper which reduces amount of logging by logging only
 * the statistics about the logged messages at specified intervals instead
 * of messages themselves.
 */
class ReducedLogger : public ILogger {
 public:
  /**
   * @param log_interval Interval at which to log the statistics.
   */
  explicit ReducedLogger(rl::LogStats log_stats,
                         absl::Duration log_interval = absl::Seconds(4));

  /**
   * @brief Adds message to statistics.
   */
  void Log(Level level, std::string message) override;

 private:
  cpp::NnSp<rl::LogStats> log_stats_;
  cpp::Timer log_stats_timer_;
};
}  // namespace vh::log

#endif  // VH_LOG_REDUCED_LOGGER_H_
