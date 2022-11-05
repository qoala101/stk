#ifndef STONKS_APP_SYMBOLS_DB_APP_SDB_TABLES_H_
#define STONKS_APP_SYMBOLS_DB_APP_SDB_TABLES_H_

#include <fmt/format.h>

#include <cstdint>
#include <gsl/assert>
#include <type_traits>

#include "cpp_type_list.h"
#include "sqldb_table.h"

namespace stonks::app::sdb::tables {
/**
 * @copydoc core::Asset
 */
struct Asset : public sqldb::Table<Asset> {
  struct id : public Column<int64_t, id> {
    struct PrimaryKey;
    struct AutoIncrement;
    struct Unique;
  };

  struct name : public Column<std::string, name> {
    struct Unique;
  };

  using Columns = cpp::TypeList<id, name>;
};

/**
 * @copydoc core::SymbolInfo
 */
struct SymbolInfo : public sqldb::Table<SymbolInfo> {
  struct id : public Column<int64_t, id> {
    struct PrimaryKey;
    struct AutoIncrement;
    struct Unique;
  };

  struct name : public Column<std::string, name> {
    struct Unique;
  };

  struct base_asset_id : public Column<int64_t, base_asset_id> {
    using ForeignKey = Asset::id;
  };

  using base_asset_min_amount = Column<double, struct base_asset_min_amount>;
  using base_asset_price_step = Column<double, struct base_asset_price_step>;

  struct quote_asset_id : public Column<int64_t, quote_asset_id> {
    using ForeignKey = Asset::id;
  };

  using quote_asset_min_amount = Column<double, struct quote_asset_min_amount>;
  using quote_asset_price_step = Column<double, struct quote_asset_price_step>;

  using Columns = cpp::TypeList<id, name, base_asset_id, base_asset_min_amount,
                                base_asset_price_step, quote_asset_id,
                                quote_asset_min_amount, quote_asset_price_step>;
};

/**
 * @copydoc core::SymbolPriceRecord
 */
struct SymbolPriceRecord : public sqldb::Table<SymbolPriceRecord> {
  struct symbol_id : public Column<int64_t, symbol_id> {
    using ForeignKey = SymbolInfo::id;
  };

  using price = Column<double, struct price>;

  struct time : public Column<int64_t, time> {
    struct Unique;
  };

  using Columns = cpp::TypeList<symbol_id, price, time>;
};
}  // namespace stonks::app::sdb::tables

#endif  // STONKS_APP_SYMBOLS_DB_APP_SDB_TABLES_H_
