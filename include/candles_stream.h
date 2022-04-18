#ifndef STONKS_CANDLESTICKS_STREAM_H_
#define STONKS_CANDLESTICKS_STREAM_H_

#include <chrono>
#include <optional>
#include <vector>

#include "finance_enums.h"
#include "finance_types.h"

namespace stonks::finance {
class CandlesStream {
 public:
  /**
   * @remark Both start and end time can be in future.
   */
  explicit CandlesStream(
      Symbol symbol, Interval interval, std::chrono::milliseconds start_time,
      std::optional<std::chrono::milliseconds> end_time = std::nullopt);

  /**
   * @remark If start time is in future, call would block until it's reached.
   * @return Empty vector if stream has ended
   * and no more data would be provided.
   */
  std::vector<Candle> GetNextCandles();

 private:
  const Symbol symbol_;
  const Interval interval_;
  std::chrono::milliseconds start_time_{};
  std::optional<std::chrono::milliseconds> end_time_{};
};
}  // namespace stonks::finance

#endif  // STONKS_CANDLESTICKS_STREAM_H_
