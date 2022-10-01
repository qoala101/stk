#ifndef STONKS_STONKS_DB_STONKS_TABLE_DEFINITIONS_H_
#define STONKS_STONKS_DB_STONKS_TABLE_DEFINITIONS_H_

#include "sqldb_types.h"

/**
 * @file Table definitions used by DB.
 */

namespace stonks::db::tables {
[[nodiscard]] auto Asset() -> const sqldb::TableDefinition &;
[[nodiscard]] auto Symbol() -> const sqldb::TableDefinition &;
[[nodiscard]] auto SymbolPriceTick() -> const sqldb::TableDefinition &;
}  // namespace stonks::db::tables

#endif  // STONKS_STONKS_DB_STONKS_TABLE_DEFINITIONS_H_