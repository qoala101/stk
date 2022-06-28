#ifndef STONKS_FINANCE_DB_TABLE_DEFINITIONS_H_
#define STONKS_FINANCE_DB_TABLE_DEFINITIONS_H_

#include "sqldb_types.h"

/**
 * @file Table definitions used by DB.
 */
namespace stonks::finance::table_definitions {
[[nodiscard]] auto Asset() -> const stonks::sqldb::TableDefinition &;
[[nodiscard]] auto Symbol() -> const stonks::sqldb::TableDefinition &;
[[nodiscard]] auto SymbolPriceTick() -> const stonks::sqldb::TableDefinition &;
}  // namespace stonks::finance::table_definitions

#endif  // STONKS_FINANCE_DB_TABLE_DEFINITIONS_H_
