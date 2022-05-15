#ifndef STONKS_STREAMS_PRICE_TICKS_DB_STREAM_H_
#define STONKS_STREAMS_PRICE_TICKS_DB_STREAM_H_

#include <cpprest/http_listener.h>

#include <chrono>
#include <optional>
#include <vector>

#include "finance_types.h"

namespace stonks::finance {
class PriceTicksDbStream {
 public:
  explicit PriceTicksDbStream(
      const std::optional<Period> &period = std::nullopt,
      int buffer_size = 10000);

  ~PriceTicksDbStream();

  auto GetNextPriceTicks() -> std::optional<std::vector<SymbolPriceTick>>;

 private:
  std::vector<SymbolPriceTick> buffer_{};
  int next_buffer_index_{};
  std::optional<std::chrono::milliseconds> last_time_{};

  const std::optional<Period> period_{};
  const int buffer_size_{};
};
}  // namespace stonks::finance

#endif  // STONKS_STREAMS_PRICE_TICKS_DB_STREAM_H_
