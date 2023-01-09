#ifndef STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SPS_BOOK_TICK_HANDLER_H_
#define STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SPS_BOOK_TICK_HANDLER_H_

#include <cppcoro/task.hpp>

#include "binance_types.h"
#include "core_i_symbols_db.h"
#include "core_types.h"
#include "cpp_not_null.h"
#include "networkx_types.h"

namespace stonks {
namespace core::sps {
/**
 * @brief Records symbol prices to Symbols DB.
 */
class BookTickHandler {
 public:
  BookTickHandler(Symbol symbol, cpp::NnUp<ISymbolsDb> symbols_db);

  /**
   * @brief Records book tick as price.
   */
  auto RecordAsPrice [[nodiscard]] (binance::BookTick book_tick)
  -> cppcoro::task<>;

 private:
  auto SymbolPriceRecordFrom
      [[nodiscard]] (const binance::BookTick &book_tick) const;

  auto GetBaseAssetPriceStep [[nodiscard]] () -> cppcoro::task<double>;

  auto GetLastPriceRecord [[nodiscard]] () -> cppcoro::task<SymbolPriceRecord>;

  Symbol symbol_{};
  cpp::NnUp<ISymbolsDb> symbols_db_;
  double base_asset_price_step_;
  SymbolPriceRecord last_record_{};
};
}  // namespace core::sps

namespace networkx {
template <>
struct WebSocketReceiverTraits<&core::sps::BookTickHandler::RecordAsPrice> {
  static constexpr auto kSynchronized = true;
};
}  // namespace networkx
}  // namespace stonks

#endif  // STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SPS_BOOK_TICK_HANDLER_H_
