#ifndef STONKS_FINANCE_TYPES_H_
#define STONKS_FINANCE_TYPES_H_

#include <chrono>
#include <string>

namespace stonks::finance {
struct Candlestick {
  std::chrono::milliseconds open_time{};
  double open_price{};
  double high_price{};
  double low_price{};
  double close_price{};
  std::chrono::milliseconds close_time{};

 private:
  friend auto operator<=>(const Candlestick &, const Candlestick &) = default;
};
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_TYPES_H_
