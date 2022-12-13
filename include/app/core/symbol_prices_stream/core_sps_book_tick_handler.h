#ifndef STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SPS_BOOK_TICK_HANDLER_H_
#define STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SPS_BOOK_TICK_HANDLER_H_

#include <cppcoro/task.hpp>

#include "binance_types.h"
#include "core_i_symbols_db.h"
#include "core_types.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"

namespace stonks::core::sps {
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

  Symbol symbol_{};
  cpp::NnUp<ISymbolsDb> symbols_db_;
  cpp::Opt<double> last_recorded_price_{};
};
}  // namespace stonks::core::sps

#endif  // STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SPS_BOOK_TICK_HANDLER_H_
