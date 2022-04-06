#ifndef STONKS_FINANCE_TYPES_H_
#define STONKS_FINANCE_TYPES_H_

#include <chrono>
#include <string>

#include "finance_enums.h"

namespace stonks::finance {
struct Symbol {
  std::string base_asset;
  std::string quote_asset;

 private:
  friend bool operator==(const Symbol &, const Symbol &) = default;
};

struct StrategyInfo {
  std::string name;

 private:
  friend bool operator==(const StrategyInfo &, const StrategyInfo &) = default;
};

struct OrderRequest {
  std::chrono::milliseconds time;
  Action action;
  Symbol symbol;
  double quantity;
  double price;

 private:
  friend bool operator==(const OrderRequest &, const OrderRequest &) = default;
};

struct Candlestick {
  std::chrono::milliseconds open_time{};
  double open_price{};
  double high_price{};
  double low_price{};
  double close_price{};
  std::chrono::milliseconds close_time{};
  double volume{};

 private:
  friend auto operator<=>(const Candlestick &, const Candlestick &) = default;
};
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_TYPES_H_
