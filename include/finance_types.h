#ifndef STONKS_FINANCE_TYPES_H_
#define STONKS_FINANCE_TYPES_H_

#include <string>

namespace stonks::finance {
struct Candlestick {
  int64_t open_time{};
  double open_price{};
  double high_price{};
  double low_price{};
  double close_price{};
  int64_t close_time{};

 private:
  friend bool operator==(const Candlestick &, const Candlestick &) = default;
};
}  // namespace stonks

#endif  // STONKS_FINANCE_TYPES_H_
