#ifndef VH_LOG_RL_LOG_STATS_H_
#define VH_LOG_RL_LOG_STATS_H_

#include <absl/container/node_hash_map.h>
#include <absl/time/time.h>

#include <atomic>
#include <string>

#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "log_types.h"

namespace vh::log::rl {
namespace detail {
struct MessageStats {
  std::string last_message_{};
  std::atomic<int> num_messages_{};
};
}  // namespace detail

/**
 * @brief Reduced logger statistics.
 */
class LogStats {
 public:
  /**
   * @param logger Logger to be used to log the statistics.
   */
  explicit LogStats(cpp::NnUp<ILogger> logger);

  /**
   * @brief Records message to statistics.
   */
  void RecordMessage(Level level, std::string message);

  /**
   * @brief Logs statistics and resets it.
   */
  void LogMessageStats();

 private:
  cpp::NnUp<ILogger> logger_;
  absl::node_hash_map<Level, detail::MessageStats> message_stats_;
  absl::Time last_log_time_{};
};
}  // namespace vh::log::rl

#endif  // VH_LOG_RL_LOG_STATS_H_
