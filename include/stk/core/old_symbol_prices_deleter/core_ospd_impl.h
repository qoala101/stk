/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_STK_CORE_OSPD_IMPL_H_
#define VH_STK_CORE_OSPD_IMPL_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>

#include "core_i_symbols_db.h"
#include "cpp_not_null.h"

namespace vh::stk::core::ospd {
/**
 * @brief Deletes old symbol prices from Symbols DB.
 */
class Impl {
 public:
  Impl(cpp::NnUp<ISymbolsDb> symbols_db,
       absl::Duration keep_prices_for_duration);

  /**
   * @brief Deletes old prices.
   */
  auto DeleteOldPrices [[nodiscard]] () const -> cppcoro::task<>;

 private:
  cpp::NnUp<ISymbolsDb> symbols_db_;
  absl::Duration keep_prices_for_duration_{};
};
}  // namespace vh::stk::core::ospd

#endif  // VH_STK_CORE_OSPD_IMPL_H_
