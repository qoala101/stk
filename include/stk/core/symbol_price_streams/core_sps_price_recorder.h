#ifndef VH_STK_CORE_SPS_PRICE_RECORDER_H_
#define VH_STK_CORE_SPS_PRICE_RECORDER_H_

#include <cppcoro/task.hpp>

#include "binance_types.h"
#include "core_i_symbols_db.h"
#include "core_types.h"
#include "cpp_not_null.h"

namespace vh::stk::core::sps {
/**
 * @brief Records symbol prices to Symbols DB.
 */
class PriceRecorder {
 public:
  PriceRecorder(Symbol symbol, cpp::NnSp<ISymbolsDb> symbols_db);

  /**
   * @brief Records book tick as price.
   */
  auto RecordAsPrice [[nodiscard]] (binance::BookTick book_tick)
  -> cppcoro::task<>;

 private:
  auto SymbolPriceRecordFrom
      [[nodiscard]] (const binance::BookTick &book_tick) const;

  Symbol symbol_{};
  cpp::NnSp<ISymbolsDb> symbols_db_;
};
}  // namespace vh::stk::core::sps

#endif  // VH_STK_CORE_SPS_PRICE_RECORDER_H_
