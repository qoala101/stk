#include "table_definitions.h"

#include <string>

#include "sqldb_enums.h"

namespace stonks::finance::table_definitions {
auto Asset() -> const sqldb::TableDefinition& {
  static const auto table_definition =
      sqldb::TableDefinition{.table = "Asset",
                             .columns = {{.column = "id",
                                          .data_type = sqldb::DataType::kInt64,
                                          .primary_key = true,
                                          .auto_increment = true,
                                          .unique = true},
                                         {.column = "name",
                                          .data_type = sqldb::DataType::kString,
                                          .unique = true}}};
  return table_definition;
}

auto Symbol() -> const sqldb::TableDefinition& {
  static const auto table_definition = sqldb::TableDefinition{
      .table = "Symbol",
      .columns = {
          {.column = "id",
           .data_type = sqldb::DataType::kInt64,
           .primary_key = true,
           .auto_increment = true,
           .unique = true},
          {.column = "name",
           .data_type = sqldb::DataType::kString,
           .unique = true},
          {.column = "base_asset_id",
           .data_type = sqldb::DataType::kInt64,
           .foreign_key = {{.table = "Asset", .column = "id"}}},
          {.column = "quote_asset_id",
           .data_type = sqldb::DataType::kInt64,
           .foreign_key = {{.table = "Asset", .column = "id"}}},
          {.column = "min_base_amount", .data_type = sqldb::DataType::kDouble},
          {.column = "min_quote_amount", .data_type = sqldb::DataType::kDouble},
          {.column = "base_step", .data_type = sqldb::DataType::kDouble},
          {.column = "quote_step", .data_type = sqldb::DataType::kDouble}}};
  return table_definition;
}

auto SymbolPriceTick() -> const sqldb::TableDefinition& {
  static const auto table_definition = sqldb::TableDefinition{
      .table = "SymbolPriceTick",
      .columns = {
          {.column = "symbol_id",
           .data_type = sqldb::DataType::kInt64,
           .foreign_key = {{.table = "Symbol", .column = "id"}}},
          {.column = "time", .data_type = sqldb::DataType::kInt64},
          {.column = "buy_price", .data_type = sqldb::DataType::kDouble},
          {.column = "sell_price", .data_type = sqldb::DataType::kDouble}}};
  return table_definition;
}
}  // namespace stonks::finance::table_definitions