#ifndef STONKS_FINANCE_TYPES_H_
#define STONKS_FINANCE_TYPES_H_

#include <chrono>
#include <optional>
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
  BuyOrSell buy_or_sell;
  Symbol symbol;
  double quantity;
  double price;

 private:
  friend bool operator==(const OrderRequest &, const OrderRequest &) = default;
};

struct StrategyOrderRequest {
  StrategyInfo strategy_info;
  OrderRequest order_request;

 private:
  friend bool operator==(const StrategyOrderRequest &,
                         const StrategyOrderRequest &) = default;
};

struct Candlestick {
  struct Data {
    double open_price{};
    double high_price{};
    double low_price{};
    double close_price{};
    double volume{};

   private:
    friend bool operator==(const Data &left, const Data &right) = default;
    friend std::partial_ordering operator<=>(const Data &left,
                                             const Data &right) = default;
  };

  std::chrono::milliseconds open_time{};
  std::chrono::milliseconds close_time{};
  std::optional<Data> data{};

 private:
  friend bool operator==(const Candlestick &left, const Candlestick &right);
  friend std::partial_ordering operator<=>(const Candlestick &left,
                                           const Candlestick &right);
};

struct TimeDouble {
  std::chrono::milliseconds time;
  double value;
};
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_TYPES_H_
