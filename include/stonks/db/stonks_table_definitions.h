#ifndef STONKS_STONKS_DB_STONKS_TABLE_DEFINITIONS_H_
#define STONKS_STONKS_DB_STONKS_TABLE_DEFINITIONS_H_

#include "sqldb_types.h"

/**
 * @file Table definitions used by DB.
 */
namespace stonks::db::table_definitions {
[[nodiscard]] auto Asset() -> const stonks::sqldb::TableDefinition &;
[[nodiscard]] auto Symbol() -> const stonks::sqldb::TableDefinition &;
[[nodiscard]] auto SymbolPriceTick() -> const stonks::sqldb::TableDefinition &;
}  // namespace stonks::db::table_definitions

#endif  // STONKS_STONKS_DB_STONKS_TABLE_DEFINITIONS_H_
