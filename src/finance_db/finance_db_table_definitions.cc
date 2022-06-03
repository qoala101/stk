#include "finance_db_table_definitions.h"

#include <optional>
#include <string>

#include "db_enums.h"

namespace stonks::finance::table_definition {
auto Asset() -> const stonks::db::TableDefinition& {
  static const auto table_definition = stonks::db::TableDefinition{
      .table = stonks::db::Table{.name = "Asset"},
      .columns = {
          stonks::db::Column{.name = "id",
                             .data_type = stonks::db::DataType::kInteger,
                             .primary_key = true,
                             .auto_increment = true,
                             .unique = true},
          stonks::db::Column{.name = "name",
                             .data_type = stonks::db::DataType::kText,
                             .unique = true}}};
  return table_definition;
}

auto Symbol() -> const stonks::db::TableDefinition& {
  static const auto table_definition = stonks::db::TableDefinition{
      .table = stonks::db::Table{.name = "Symbol"},
      .columns = {
          stonks::db::Column{.name = "id",
                             .data_type = stonks::db::DataType::kInteger,
                             .primary_key = true,
                             .auto_increment = true,
                             .unique = true},
          stonks::db::Column{.name = "name",
                             .data_type = stonks::db::DataType::kText,
                             .unique = true},
          stonks::db::Column{
              .name = "base_asset_id",
              .data_type = stonks::db::DataType::kInteger,
              .foreign_key = stonks::db::ForeignKey{.table_name = "Asset",
                                                    .column_name = "id"}},
          stonks::db::Column{
              .name = "quote_asset_id",
              .data_type = stonks::db::DataType::kInteger,
              .foreign_key = stonks::db::ForeignKey{.table_name = "Asset",
                                                    .column_name = "id"}},
          stonks::db::Column{.name = "min_base_amount",
                             .data_type = stonks::db::DataType::kReal},
          stonks::db::Column{.name = "min_quote_amount",
                             .data_type = stonks::db::DataType::kReal},
          stonks::db::Column{.name = "base_step",
                             .data_type = stonks::db::DataType::kReal},
          stonks::db::Column{.name = "quote_step",
                             .data_type = stonks::db::DataType::kReal}}};
  return table_definition;
}

auto SymbolPriceTick() -> const stonks::db::TableDefinition& {
  static const auto table_definition = stonks::db::TableDefinition{
      .table = stonks::db::Table{.name = "SymbolPriceTick"},
      .columns = {
          stonks::db::Column{
              .name = "symbol_id",
              .data_type = stonks::db::DataType::kInteger,
              .foreign_key = stonks::db::ForeignKey{.table_name = "Symbol",
                                                    .column_name = "id"}},
          stonks::db::Column{.name = "time",
                             .data_type = stonks::db::DataType::kInteger},
          stonks::db::Column{.name = "buy_price",
                             .data_type = stonks::db::DataType::kReal},
          stonks::db::Column{.name = "sell_price",
                             .data_type = stonks::db::DataType::kReal}}};
  return table_definition;
}
}  // namespace stonks::finance::table_definition