#include "app_sdb_tables.h"

#include <string>

#include "cpp_typed_struct.h"
#include "sqldb_enums.h"

namespace stonks::app::sdb::tables {
auto Asset() -> const sqldb::TableDefinition& {
  static const auto kTable =
      sqldb::TableDefinition{.table = {"Asset"},
                             .columns = {{.column = {"id"},
                                          .data_type = sqldb::DataType::kInt64,
                                          .primary_key = true,
                                          .auto_increment = true,
                                          .unique = true},
                                         {.column = {"name"},
                                          .data_type = sqldb::DataType::kString,
                                          .unique = true}}};
  return kTable;
}

auto SymbolInfo() -> const sqldb::TableDefinition& {
  static const auto kTable = sqldb::TableDefinition{
      .table = {"SymbolInfo"},
      .columns = {{.column = {"id"},
                   .data_type = sqldb::DataType::kInt64,
                   .primary_key = true,
                   .auto_increment = true,
                   .unique = true},
                  {.column = {"name"},
                   .data_type = sqldb::DataType::kString,
                   .unique = true},
                  {.column = {"base_asset_id"},
                   .data_type = sqldb::DataType::kInt64,
                   .foreign_key = {{.table = {"Asset"}, .column = {"id"}}}},
                  {.column = {"min_base_amount"},
                   .data_type = sqldb::DataType::kDouble},
                  {.column = {"base_price_step"},
                   .data_type = sqldb::DataType::kDouble},
                  {.column = {"quote_asset_id"},
                   .data_type = sqldb::DataType::kInt64,
                   .foreign_key = {{.table = {"Asset"}, .column = {"id"}}}},
                  {.column = {"min_quote_amount"},
                   .data_type = sqldb::DataType::kDouble},
                  {.column = {"quote_price_step"},
                   .data_type = sqldb::DataType::kDouble}}};
  return kTable;
}

auto SymbolPriceRecord() -> const sqldb::TableDefinition& {
  static const auto kTable = sqldb::TableDefinition{
      .table = {"SymbolPriceRecord"},
      .columns = {{.column = {"symbol_id"},
                   .data_type = sqldb::DataType::kInt64,
                   .foreign_key = {{.table = {"Symbol"}, .column = {"id"}}}},
                  {.column = {"price"}, .data_type = sqldb::DataType::kDouble},
                  {.column = {"time"}, .data_type = sqldb::DataType::kInt64}}};
  return kTable;
}
}  // namespace stonks::app::sdb::tables