/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_STK_CORE_SIU_IMPL_H_
#define VH_STK_CORE_SIU_IMPL_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>

#include "binance_api.h"
#include "core_i_symbols_db.h"
#include "cpp_not_null.h"

namespace vh::stk::core::siu {
/**
 * @brief Updates assets and symbols info in DB.
 */
class Impl {
 public:
  Impl(cpp::NnUp<ISymbolsDb> symbols_db, binance::BinanceApi binance_api,
       absl::Duration update_symbols_info_interval);

  /**
   * @brief Receives symbols info from Binance and update Symbols DB if needed.
   */
  auto GetAndUpdateSymbolsInfo [[nodiscard]] () -> cppcoro::task<>;

 private:
  cpp::NnUp<ISymbolsDb> symbols_db_;
  binance::BinanceApi binance_api_;
  absl::Duration update_symbols_info_interval_{};
  absl::Time last_update_time_{};
};
}  // namespace vh::stk::core::siu

#endif  // VH_STK_CORE_SIU_IMPL_H_
