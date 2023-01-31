#ifndef STONKS_APP_CORE_SYMBOLS_DB_CORE_SDB_TABLES_H_
#define STONKS_APP_CORE_SYMBOLS_DB_CORE_SDB_TABLES_H_

#include <cstdint>
#include <gsl/assert>
#include <type_traits>

#include "core_types.h"
#include "cpp_type_list.h"
#include "sqldb_table.h"

namespace stonks::core::sdb::tables {
/**
 * @copydoc Asset
 */
struct Asset : public sqldb::Table<Asset> {
  struct id : public Column<int64_t, id> {
    struct PrimaryKey;
    struct AutoIncrement;
    struct Unique;
  };

  struct name : public Column<core::Asset::ValueType, name> {
    struct Unique;
  };

  using Columns = cpp::TypeList<id, name>;
};

/**
 * @copydoc SymbolInfo
 */
struct SymbolInfo : public sqldb::Table<SymbolInfo> {
  struct id : public Column<int64_t, id> {
    struct PrimaryKey;
    struct AutoIncrement;
    struct Unique;
  };

  struct name
      : public Column<decltype(core::SymbolInfo::symbol)::ValueType, name> {
    struct Unique;
  };

  struct base_asset_id : public Column<int64_t, base_asset_id> {
    using ForeignKey = Asset::id;
  };

  using base_asset_min_amount =
      Column<decltype(core::SymbolInfo{}.base_asset.min_amount),
             struct base_asset_min_amount>;

  using base_asset_price_step =
      Column<decltype(core::SymbolInfo{}.base_asset.price_step),
             struct base_asset_price_step>;

  struct quote_asset_id : public Column<int64_t, quote_asset_id> {
    using ForeignKey = Asset::id;
  };

  using quote_asset_min_amount =
      Column<decltype(core::SymbolInfo{}.quote_asset.min_amount),
             struct quote_asset_min_amount>;

  using quote_asset_price_step =
      Column<decltype(core::SymbolInfo{}.quote_asset.price_step),
             struct quote_asset_price_step>;

  using Columns = cpp::TypeList<id, name, base_asset_id, base_asset_min_amount,
                                base_asset_price_step, quote_asset_id,
                                quote_asset_min_amount, quote_asset_price_step>;
};

/**
 * @copydoc SymbolPriceRecord
 */
struct SymbolPriceRecord : public sqldb::Table<SymbolPriceRecord> {
  struct symbol_id : public Column<int64_t, symbol_id> {
    using ForeignKey = SymbolInfo::id;
  };

  using buy_price =
      Column<decltype(core::SymbolPriceRecord::buy_price)::ValueType,
             struct buy_price>;

  using sell_price =
      Column<decltype(core::SymbolPriceRecord::sell_price)::ValueType,
             struct sell_price>;

  struct time : public Column<int64_t, struct time> {};

  using Columns = cpp::TypeList<symbol_id, buy_price, sell_price, time>;
};
}  // namespace stonks::core::sdb::tables

#endif  // STONKS_APP_CORE_SYMBOLS_DB_CORE_SDB_TABLES_H_
