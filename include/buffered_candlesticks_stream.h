#ifndef STONKS_BUFFERED_CANDLESTICKS_STREAM_H_
#define STONKS_BUFFERED_CANDLESTICKS_STREAM_H_

#include <vector>

#include "candlesticks_stream.h"
#include "finance_types.h"

namespace stonks::finance {
class BufferedCandlesticksStream {
 public:
  explicit BufferedCandlesticksStream(CandlesticksStream stream,
                                      int max_candlesticks_per_request);

  std::vector<Candlestick> GetNextCandlesticks();

 private:
  CandlesticksStream stream_;
  const int max_candlesticks_per_request_;
  std::vector<Candlestick> buffer_{};
  int next_buffer_index_{};
};
}  // namespace stonks::finance

#endif  // STONKS_BUFFERED_CANDLESTICKS_STREAM_H_
