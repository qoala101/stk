#include "finance_types.h"

#include <tuple>

namespace stonks::finance {
bool operator==(const Candlestick &left, const Candlestick &right) {
  const auto equal = std::tie(left.open_time, left.close_time) ==
                     std::tie(right.open_time, right.close_time);

  if (!equal) {
    return equal;
  }

  return left.data.value_or(Candlestick::Data{}) ==
         right.data.value_or(Candlestick::Data{});
}

std::partial_ordering operator<=>(const Candlestick &left,
                                  const Candlestick &right) {
  const auto ordering = std::tie(left.open_time, left.close_time) <=>
                        std::tie(right.open_time, right.close_time);

  if (ordering != std::partial_ordering::equivalent) {
    return ordering;
  }

  return left.data.value_or(Candlestick::Data{}) <=>
         right.data.value_or(Candlestick::Data{});
}
};  // namespace stonks::finance