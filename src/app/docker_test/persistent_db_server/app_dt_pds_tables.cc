#include "app_dt_pds_tables.h"

#include <string>

#include "cpp_typed_struct.h"
#include "sqldb_enums.h"

namespace stonks::app::dt::pds::tables {
auto SymbolPriceRecord() -> const sqldb::TableDefinition& {
  static const auto table = sqldb::TableDefinition{
      .table = {"SymbolPriceRecord"},
      .columns = {
          {.column = {"id"},
           .data_type = sqldb::DataType::kInt64,
           .primary_key = true,
           .auto_increment = true,
           .unique = true},
          {.column = {"time"}, .data_type = sqldb::DataType::kInt64},
          {.column = {"symbol"}, .data_type = sqldb::DataType::kString},
          {.column = {"price"}, .data_type = sqldb::DataType::kDouble}}};
  return table;
}
}  // namespace stonks::app::dt::pds::tables