#ifndef STONKS_APP_CORE_SYMBOL_PRICES_FILTER_CORE_SYMBOL_PRICES_FILTER_H_
#define STONKS_APP_CORE_SYMBOL_PRICES_FILTER_CORE_SYMBOL_PRICES_FILTER_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>
#include <vector>

#include "core_sdb_proxy.h"
#include "core_types.h"

namespace stonks::core {
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
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_SYMBOL_PRICES_FILTER_CORE_SYMBOL_PRICES_FILTER_H_
