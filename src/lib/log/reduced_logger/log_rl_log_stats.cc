#include "log_rl_log_stats.h"

#include <absl/hash/hash.h>
#include <absl/time/clock.h>
#include <fmt/core.h>

#include <gsl/assert>
#include <magic_enum.hpp>
#include <not_null.hpp>
#include <utility>

namespace vh::log::rl {
LogStats::LogStats(cpp::NnUp<ILogger> logger)
    : logger_{std::move(logger)},
      message_stats_{[]() {
        auto message_stats = absl::node_hash_map<Level, detail::MessageStats>();

        magic_enum::enum_for_each<Level>(
            [&message_stats](auto level) { message_stats[level]; });

        return message_stats;
      }()},
      last_log_time_{absl::Now()} {
  Ensures(message_stats_.size() == magic_enum::enum_count<Level>());
}

void LogStats::RecordMessage(Level level, std::string message) {
  auto &message_stats = message_stats_.at(level);
  message_stats.last_message_ = std::move(message);
  message_stats.num_messages_.fetch_add(1, std::memory_order_relaxed);
}

void LogStats::LogMessageStats() {
  for (auto &[level, message_stats] : message_stats_) {
    const auto num_messages =
        message_stats.num_messages_.exchange(0, std::memory_order_relaxed);

    if (num_messages <= 0) {
      continue;
    }

    if (num_messages == 1) {
      logger_->Log(level, message_stats.last_message_);
      continue;
    }

    const auto time_from_last_log = absl::Now() - last_log_time_;

    logger_->Log(level, fmt::format("In last {} logged {} messages. Last: {}",
                                    absl::FormatDuration(time_from_last_log),
                                    num_messages, message_stats.last_message_));
  }

  last_log_time_ = absl::Now();
}
}  // namespace vh::log::rl
