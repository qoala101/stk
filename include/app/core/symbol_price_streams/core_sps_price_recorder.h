#ifndef STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SPS_PRICE_RECORDER_H_
#define STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SPS_PRICE_RECORDER_H_

#include <cppcoro/task.hpp>

#include "binance_types.h"
#include "core_i_symbols_db.h"
#include "core_types.h"
#include "cpp_not_null.h"

namespace stonks::core::sps {
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
  auto GetBaseAssetPriceStep [[nodiscard]] () const -> cppcoro::task<double>;

  auto GetLastPriceRecord [[nodiscard]] () const
      -> cppcoro::task<cpp::Opt<SymbolPriceRecord>>;

  auto SymbolPriceRecordFrom [[nodiscard]] (const binance::BookTick &book_tick,
                                            double base_asset_price_step);

  Symbol symbol_{};
  cpp::NnSp<ISymbolsDb> symbols_db_;
};
}  // namespace stonks::core::sps

#endif  // STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SPS_PRICE_RECORDER_H_
