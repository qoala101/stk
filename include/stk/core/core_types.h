#ifndef STONKS_CORE_CORE_TYPES_H_
#define STONKS_CORE_CORE_TYPES_H_

#include <absl/time/time.h>

#include <string>

#include "cpp_typed_struct.h"

namespace vh::stk::core {
/**
 * @brief Currency.
 */
struct Asset : public cpp::TypedStruct<std::string> {};

/**
 * @brief Name of the pair of assets.
 */
struct Symbol : public cpp::TypedStruct<std::string> {};

/**
 * @brief Trading info related to asset.
 */
struct AssetInfo {
  Asset asset{};
  double min_amount{};
  double price_step{};

 private:
  friend auto operator== [[nodiscard]] (const AssetInfo &, const AssetInfo &)
  -> bool = default;
  friend auto operator<=> [[nodiscard]] (const AssetInfo &, const AssetInfo &)
  -> std::partial_ordering = default;
};

/**
 * @brief Trading info related to symbol.
 */
struct SymbolInfo {
  Symbol symbol{};
  AssetInfo base_asset{};
  AssetInfo quote_asset{};

 private:
  friend auto operator== [[nodiscard]] (const SymbolInfo &, const SymbolInfo &)
  -> bool = default;
  friend auto operator<=> [[nodiscard]] (const SymbolInfo &, const SymbolInfo &)
  -> std::partial_ordering = default;
};

/**
 * @brief Asset amount.
 */
struct Price : public cpp::TypedStruct<double> {};

/**
 * @brief Price of the symbol at specified time.
 */
struct SymbolPriceRecord {
  Symbol symbol{};
  Price buy_price{};
  Price sell_price{};
  absl::Time time{};

 private:
  friend auto operator==
      [[nodiscard]] (const SymbolPriceRecord &, const SymbolPriceRecord &)
      -> bool = default;
  friend auto operator<=>
      [[nodiscard]] (const SymbolPriceRecord &, const SymbolPriceRecord &)
      -> std::partial_ordering = default;
};

/**
 * @brief Order of time sorting.
 */
enum class TimeOrder { kNewFirst, kOldFirst };
}  // namespace vh::stk::core

#endif  // STONKS_CORE_CORE_TYPES_H_
