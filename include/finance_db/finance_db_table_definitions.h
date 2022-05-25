#ifndef STONKS_FINANCE_DB_FINANCE_DB_TABLE_DEFINITIONS_H_
#define STONKS_FINANCE_DB_FINANCE_DB_TABLE_DEFINITIONS_H_

#include "db_types.h"

namespace stonks::finance::table_definition {
auto Asset() -> const stonks::db::TableDefinition &;
auto Symbol() -> const stonks::db::TableDefinition &;
auto SymbolPriceTick() -> const stonks::db::TableDefinition &;
}  // namespace stonks::finance::table_definition

#endif  // STONKS_FINANCE_DB_FINANCE_DB_TABLE_DEFINITIONS_H_
