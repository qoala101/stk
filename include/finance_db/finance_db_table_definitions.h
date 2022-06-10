#ifndef STONKS_FINANCE_DB_FINANCE_DB_TABLE_DEFINITIONS_H_
#define STONKS_FINANCE_DB_FINANCE_DB_TABLE_DEFINITIONS_H_

#include "sqldb_types.h"

namespace stonks::finance::table_definition {
auto Asset() -> const stonks::sqldb::TableDefinition &;
auto Symbol() -> const stonks::sqldb::TableDefinition &;
auto SymbolPriceTick() -> const stonks::sqldb::TableDefinition &;
}  // namespace stonks::finance::table_definition

#endif  // STONKS_FINANCE_DB_FINANCE_DB_TABLE_DEFINITIONS_H_
