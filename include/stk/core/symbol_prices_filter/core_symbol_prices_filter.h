#ifndef VH_STK_CORE_SYMBOL_PRICES_FILTER_H_
#define VH_STK_CORE_SYMBOL_PRICES_FILTER_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>
#include <vector>

#include "core_sdb_proxy.h"
#include "core_types.h"

namespace vh::stk::core {
/**
 * @brief Filters inserted symbol prices.
 */
class SymbolPricesFilter : public sdb::Proxy {
 public:
  using Proxy::Proxy;

  /**
   * @brief Inserts the record only if price differs from the last one by
   * the price step amount.
   */
  auto InsertSymbolPriceRecord [[nodiscard]] (SymbolPriceRecord record)
  -> cppcoro::task<> override;

 private:
  auto GetBaseAssetPriceStep [[nodiscard]] (const Symbol &symbol) const
      -> cppcoro::task<double>;

  auto GetLastPriceRecord [[nodiscard]] (const Symbol &symbol) const
      -> cppcoro::task<cpp::Opt<SymbolPriceRecord>>;
};
}  // namespace vh::stk::core

#endif  // VH_STK_CORE_SYMBOL_PRICES_FILTER_H_
