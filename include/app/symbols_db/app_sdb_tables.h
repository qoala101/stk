#ifndef STONKS_APP_SYMBOLS_DB_APP_SDB_TABLES_H_
#define STONKS_APP_SYMBOLS_DB_APP_SDB_TABLES_H_

#include <fmt/format.h>

#include <gsl/assert>
#include <nameof.hpp>
#include <type_traits>

#include "cpp_optional.h"
#include "sqldb_types.h"

namespace stonks::app::sdb::tables {
/**
 * @copydoc core::Asset
 */
struct Asset {
  struct id {
    using Table = Asset;
    using DataType = int64_t;
    struct PrimaryKey;
    struct AutoIncrement;
    struct Unique;
  };

  struct name {
    using Table = Asset;
    using DataType = std::string;
    struct Unique;
  };

  using Columns = std::tuple<id, name>;
};

/**
 * @copydoc core::SymbolInfo
 */
struct SymbolInfo {
  struct id {
    using Table = SymbolInfo;
    using DataType = int64_t;
    struct PrimaryKey;
    struct AutoIncrement;
    struct Unique;
  };

  struct name {
    using Table = SymbolInfo;
    using DataType = std::string;
    struct Unique;
  };

  struct base_asset_id {
    using Table = SymbolInfo;
    using DataType = int64_t;
    using ForeignKey = Asset::id;
  };

  struct base_asset_min_amount {
    using Table = SymbolInfo;
    using DataType = double;
  };

  struct base_asset_price_step {
    using Table = SymbolInfo;
    using DataType = double;
  };

  struct quote_asset_id {
    using Table = SymbolInfo;
    using DataType = int64_t;
    using ForeignKey = Asset::id;
  };

  struct quote_asset_min_amount {
    using Table = SymbolInfo;
    using DataType = double;
  };

  struct quote_asset_price_step {
    using Table = SymbolInfo;
    using DataType = double;
  };

  using Columns = std::tuple<id, name, base_asset_id, base_asset_min_amount,
                             base_asset_price_step, quote_asset_id,
                             quote_asset_min_amount, quote_asset_price_step>;
};

/**
 * @copydoc core::SymbolPriceRecord
 */
struct SymbolPriceRecord {
  struct symbol_id {
    using Table = SymbolPriceRecord;
    using DataType = int64_t;
    using ForeignKey = SymbolInfo::id;
  };

  struct price {
    using Table = SymbolPriceRecord;
    using DataType = double;
  };

  struct time {
    using Table = SymbolPriceRecord;
    using DataType = int64_t;
    struct Unique;
  };

  using Columns = std::tuple<symbol_id, price, time>;
};
}  // namespace stonks::app::sdb::tables

#endif  // STONKS_APP_SYMBOLS_DB_APP_SDB_TABLES_H_
