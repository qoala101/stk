#ifndef STONKS_STRATEGIES_STRATEGY_H_
#define STONKS_STRATEGIES_STRATEGY_H_

#include <boost/uuid/uuid.hpp>
#include <chrono>
#include <vector>

#include "finance_enums.h"
#include "finance_types.h"

namespace stonks {
/**
 * @brief Description of data required to run the strategy.
 */
struct Inputs {
  finance::SymbolName symbol{};
  bool price_ticks{};
  bool order_updates{};
};

/**
 * @brief Order request.
 */
struct OrderRequest {
  std::chrono::milliseconds time{};
  boost::uuids::uuid order_uuid{};
  finance::SymbolName symbol{};
  finance::Action action{};
  finance::Amount amount{};
  finance::OrderType type{};
};

/**
 * @brief Order state change.
 */
struct OrderState {
  std::chrono::milliseconds time{};
  boost::uuids::uuid order_uuid{};
  finance::OrderUpdate state{};
};

/**
 * @brief Order request with all of it's updates.
 */
struct Order {
  OrderRequest request{};
  std::vector<OrderState> states{};

  [[nodiscard]] auto IsValidToBeFilled() const -> bool;
};

/**
 * @brief New information for the strategy to process and react to.
 */
struct Event {
  std::chrono::milliseconds time{};
  std::vector<finance::SymbolPriceTick> price_ticks{};
  std::vector<OrderState> updated_orders{};
};

class Strategy {
 public:
  virtual ~Strategy() = default;

  [[nodiscard]] auto Run(const finance::SymbolName &symbol)
      -> std::vector<finance::SymbolBalanceTick>;

 protected:
  explicit Strategy(Inputs required_data);

  void PostOrder(OrderRequest order_request);

  [[nodiscard]] auto GetSymbolInfo() const -> const finance::SymbolInfo &;

 private:
  virtual void ProcessEvent(const Event &event);

  Inputs inputs_{};
  std::vector<finance::SymbolPriceTick> price_ticks_{};
  std::vector<finance::SymbolInfo> symbols_info_{};

  std::vector<Order> orders_{};
};
}  // namespace stonks

#endif  // STONKS_STRATEGIES_STRATEGY_H_
