#ifndef STONKS_FINANCE_TYPES_H_
#define STONKS_FINANCE_TYPES_H_

#include <boost/uuid/uuid.hpp>
#include <chrono>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "finance_enums.h"

namespace stonks::finance {
struct Symbol {
  std::string base_asset{};
  std::string quote_asset{};

 private:
  friend bool operator==(const Symbol &, const Symbol &) = default;
};

struct Candle {
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
  friend bool operator==(const Candle &left, const Candle &right);
  friend std::partial_ordering operator<=>(const Candle &left,
                                           const Candle &right);
};

struct TimeDouble {
  std::chrono::milliseconds time{};
  double value{};
};

struct BreakoutStrategyData {
  std::chrono::milliseconds last_candle_close_time{};
  double expected_price{};
};

/**
 * @brief Relevant strategy information which could be used for analysis.
 */
using StrategyDataVariant = std::variant<BreakoutStrategyData>;

struct StrategyData {
  StrategyDataVariant strategy_data{};

  std::optional<std::string> GetName() const;
};

struct MarketOrderType {};

struct LimitOrderType {
  double price{};
};

using OrderTypeVariant = std::variant<MarketOrderType, LimitOrderType>;

struct OrderType {
  OrderTypeVariant order_type{};

  std::optional<double> GetPrice() const;
};

struct OrderInfo {
  OrderStatus order_status{};
  double requested_amount{};
  double executed_amount{};
  double price{};
};

struct OrderError {
  std::string message{};
};

using OrderUpdateVariant = std::variant<OrderError, OrderInfo>;

struct OrderUpdate {
  OrderUpdateVariant order_update{};

  std::optional<std::string> GetErrorMessage() const;
  std::optional<OrderStatus> GetOrderStatus() const;
};

/**
 * @brief Data stored for each order by the order proxy for each update
 * received from the order monitor.
 */
struct OrderProxyOrderUpdate {
  std::chrono::milliseconds received_time{};
  OrderUpdate order_update{};
};

struct SpecificAmount {
  double amount{};
};

struct UnspecifiedAmount {};

using AmountVariant = std::variant<UnspecifiedAmount, SpecificAmount>;

struct Amount {
  AmountVariant amount{};

  std::optional<double> GetAmount() const;
};

struct Order {
  /*
   * Data generated when order request is received.
   */
  boost::uuids::uuid uuid{};
  std::chrono::milliseconds request_time{};
  /*
   * Data specified by order creator
   */
  Symbol symbol{};
  BuyOrSell buy_or_sell{};
  Amount amount{};
  OrderType order_type{};
  /**
   * @brief Updates received after order is sent to processing.
   * Current order state could be identified by the last update.
   * Update time is set when update is received.
   */
  std::vector<OrderProxyOrderUpdate> order_updates{};
  /**
   * @brief Data describing which strategy generated the order and why.
   * Would be used for strategy analysis.
   */
  StrategyData strategy_data{};

  std::optional<OrderInfo> GetLastFilledOrderInfo() const;
};

/**
 * @brief Data sent by the strategies to order proxy which is used to create the
 * order.
 */
struct StrategyOrderRequest {
  Symbol symbol{};
  BuyOrSell buy_or_sell{};
  Amount amount{};
  OrderType order_type{};

  StrategyData strategy_data{};
};

/**
 * @brief Data sent from the order proxy to the order monitor to place the
 * order.
 */
struct OrderProxyOrderRequest {
  boost::uuids::uuid order_uuid{};

  Symbol symbol{};
  BuyOrSell buy_or_sell{};
  Amount amount{};
  OrderType order_type{};
};

/**
 * @brief Data sent by the order proxy to the order monitor to tell that order
 * should be monitored.
 */
struct OrderProxyMonitorRequest {
  boost::uuids::uuid order_uuid{};
  Symbol symbol{};
  /**
   * @brief Last order state which proxy has. Nullopt if order proxy doesn't
   * have any updates for the order.
   */
  std::optional<OrderUpdate> last_order_update{};
};

/**
 * @brief Data sent by the order monitor to the order proxy when order monitor
 * observes order update.
 */
struct OrderMonitorOrderUpdate {
  boost::uuids::uuid order_uuid{};
  OrderUpdate order_update{};
};

/**
 * @brief Data stored by the order monitor to check whether order state is
 * changed.
 */
struct OrderMonitorOrderState {
  boost::uuids::uuid order_uuid{};
  Symbol symbol{};
  std::optional<OrderUpdate> last_order_update{};
};
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_TYPES_H_
