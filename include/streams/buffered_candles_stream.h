#ifndef STONKS_STREAMS_BUFFERED_CANDLES_STREAM_H_
#define STONKS_STREAMS_BUFFERED_CANDLES_STREAM_H_

#include <vector>

#include "candles_stream.h"
#include "finance_types.h"

namespace stonks::finance {
class BufferedCandlesStream {
 public:
  explicit BufferedCandlesStream(CandlesStream stream,
                                 int max_candles_per_request);

  std::vector<Candle> GetNextCandles();

 private:
  CandlesStream stream_;
  const int max_candles_per_request_;
  std::vector<Candle> buffer_{};
  int next_buffer_index_{};
};
}  // namespace stonks::finance

#endif  // STONKS_STREAMS_BUFFERED_CANDLES_STREAM_H_
