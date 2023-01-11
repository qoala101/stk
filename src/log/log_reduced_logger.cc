#include "log_reduced_logger.h"

#include <absl/time/time.h>

#include <not_null.hpp>
#include <utility>

#include "cpp_not_null.h"
#include "cpp_share.h"
#include "cpp_timer_builder.h"
#include "log_rl_log_stats.h"
#include "log_types.h"

namespace stonks::log {
ReducedLogger::ReducedLogger(rl::LogStats log_stats,
                             absl::Duration log_interval)
    : log_stats_{cpp::Share(std::move(log_stats))},
      log_stats_timer_{cpp::Execute([log_stats = log_stats_]() {
                         log_stats->LogMessageStats();
                       })
                           .Every(log_interval)
                           .Start()} {}

void ReducedLogger::Log(Level level, std::string message) {
  log_stats_->RecordMessage(level, std::move(message));
}
}  // namespace stonks::log