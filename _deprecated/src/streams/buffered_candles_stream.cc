#include "buffered_candles_stream.h"

#include <gsl/assert>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/subrange.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/view.hpp>
#include <utility>

namespace stonks::finance {
BufferedCandlesStream::BufferedCandlesStream(CandlesStream stream,
                                             int max_candles_per_request)
    : stream_{std::move(stream)},
      max_candles_per_request_{max_candles_per_request},
      next_buffer_index_{0} {
  Expects(max_candles_per_request > 0);
}

std::vector<Candle> BufferedCandlesStream::GetNextCandles() {
  if (buffer_.empty()) {
    const auto candles = stream_.GetNextCandles();
    const auto result = candles |
                        ranges::views::take(max_candles_per_request_) |
                        ranges::to_vector;
    buffer_ = candles | ranges::views::drop(max_candles_per_request_) |
              ranges::to_vector;
    return result;
  }

  const auto result = buffer_ | ranges::views::drop(next_buffer_index_) |
                      ranges::views::take(max_candles_per_request_) |
                      ranges::to_vector;
  next_buffer_index_ += max_candles_per_request_;

  if (const auto next_index_out_of_buffer =
          next_buffer_index_ >= buffer_.size()) {
    buffer_.clear();
    next_buffer_index_ = 0;
  }

  return result;
}
}  // namespace stonks::finance