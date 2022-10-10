#ifndef STONKS_APP_SYMBOLS_DB_APP_SDB_TABLES_H_
#define STONKS_APP_SYMBOLS_DB_APP_SDB_TABLES_H_

#include "sqldb_types.h"

namespace stonks::app::sdb::tables {
/**
 * @copydoc core::Asset
 */
struct Asset {
  [[nodiscard]] static auto Definition() -> const sqldb::TableDefinition &;

  static constexpr auto kTable = "Asset";
  static constexpr auto kId = "id";
  static constexpr auto kName = "name";
};

/**
 * @copydoc core::SymbolInfo
 */
struct SymbolInfo {
  [[nodiscard]] static auto Definition() -> const sqldb::TableDefinition &;

  static constexpr auto kTable = "SymbolInfo";
  static constexpr auto kId = "id";
  static constexpr auto kName = "name";
  static constexpr auto kBaseAssetId = "base_asset_id";
  static constexpr auto kBaseAssetMinAmount = "base_asset_min_amount";
  static constexpr auto kBaseAssetPriceStep = "base_asset_price_step";
  static constexpr auto kQuoteAssetId = "quote_asset_id";
  static constexpr auto kQuoteAssetMinAmount = "quote_asset_min_amount";
  static constexpr auto kQuoteAssetPriceStep = "quote_asset_price_step";
};

/**
 * @copydoc core::SymbolPriceRecord
 */
struct SymbolPriceRecord {
  [[nodiscard]] static auto Definition() -> const sqldb::TableDefinition &;

  static constexpr auto kTable = "SymbolPriceRecord";
  static constexpr auto kSymbolId = "symbol_id";
  static constexpr auto kPrice = "price";
  static constexpr auto kTime = "time";
};
}  // namespace stonks::app::sdb::tables

#endif  // STONKS_APP_SYMBOLS_DB_APP_SDB_TABLES_H_
