#include "buffered_candlesticks_stream.h"

#include <gsl/assert>
#include <range/v3/to_container.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/take.hpp>

namespace stonks::finance {
BufferedCandlesticksStream::BufferedCandlesticksStream(
    CandlesticksStream stream, int max_candlesticks_per_request)
    : stream_{std::move(stream)},
      max_candlesticks_per_request_{max_candlesticks_per_request},
      next_buffer_index_{0} {
  Expects(max_candlesticks_per_request > 0);
}

std::vector<Candlestick> BufferedCandlesticksStream::GetNextCandlesticks() {
  if (buffer_.empty()) {
    const auto candlesticks = stream_.GetNextCandlesticks();
    const auto result = candlesticks |
                        ranges::views::take(max_candlesticks_per_request_) |
                        ranges::to_vector;
    buffer_ = candlesticks |
              ranges::views::drop(max_candlesticks_per_request_) |
              ranges::to_vector;
    return result;
  }

  const auto result = buffer_ | ranges::views::drop(next_buffer_index_) |
                      ranges::views::take(max_candlesticks_per_request_) |
                      ranges::to_vector;
  next_buffer_index_ += max_candlesticks_per_request_;

  if (const auto next_index_out_of_buffer =
          next_buffer_index_ >= buffer_.size()) {
    buffer_.clear();
    next_buffer_index_ = 0;
  }

  return result;
}
}  // namespace stonks::finance