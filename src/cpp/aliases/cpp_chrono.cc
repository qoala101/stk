#include "cpp_chrono.h"

namespace stonks::cpp {
auto GetCurrentTime() -> TimePoint {
  const auto current_time = std::chrono::system_clock::now();
  return std::chrono::duration_cast<TimePoint>(current_time.time_since_epoch());
}
}  // namespace stonks::cpp