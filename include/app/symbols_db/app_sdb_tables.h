#ifndef STONKS_APP_SYMBOLS_DB_APP_SDB_TABLES_H_
#define STONKS_APP_SYMBOLS_DB_APP_SDB_TABLES_H_

#include "sqldb_types.h"

namespace stonks::app::sdb::tables {
/**
 * @copydoc core::Asset
 */
[[nodiscard]] auto Asset() -> const sqldb::TableDefinition &;

/**
 * @copydoc core::SymbolInfo
 */
[[nodiscard]] auto SymbolInfo() -> const sqldb::TableDefinition &;

/**
 * @copydoc core::SymbolPriceRecord
 */
[[nodiscard]] auto SymbolPriceRecord() -> const sqldb::TableDefinition &;
}  // namespace stonks::app::sdb::tables

#endif  // STONKS_APP_SYMBOLS_DB_APP_SDB_TABLES_H_
