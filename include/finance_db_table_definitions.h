#ifndef STONKS_FINANCE_DB_TABLE_DEFINITIONS_H_
#define STONKS_FINANCE_DB_TABLE_DEFINITIONS_H_

#include "db_types.h"

namespace stonks::finance {
static const auto kAssetTableDefinition = stonks::db::TableDefinition{
    .table = stonks::db::Table{.name = "Asset"},
    .columns = {
        stonks::db::Column{.name = "id",
                           .data_type = stonks::db::DataType::kInteger,
                           .primary_key = true,
                           .auto_increment = true,
                           .unique = true},
        stonks::db::Column{.name = "name",
                           .data_type = stonks::db::DataType::kText,
                           .unique = true},
    }};

static const auto kSymbolTableDefinition = stonks::db::TableDefinition{
    .table = stonks::db::Table{.name = "Symbol"},
    .columns = {
        stonks::db::Column{.name = "id",
                           .data_type = stonks::db::DataType::kInteger,
                           .primary_key = true,
                           .auto_increment = true,
                           .unique = true},
        stonks::db::Column{
            .name = "base_asset_id",
            .data_type = stonks::db::DataType::kInteger,
            .foreign_key = stonks::db::ForeignKey{.table_name = "Asset",
                                                  .column_name = "id"}},
        stonks::db::Column{.name = "quote_asset_id",
                           .data_type = stonks::db::DataType::kInteger,
                           .foreign_key = stonks::db::ForeignKey{
                               .table_name = "Asset", .column_name = "id"}}}};

static const auto kSymbolPriceTableDefinition = stonks::db::TableDefinition{
    .table = stonks::db::Table{.name = "SymbolPrice"},
    .columns = {
        stonks::db::Column{
            .name = "symbol_id",
            .data_type = stonks::db::DataType::kInteger,
            .foreign_key = stonks::db::ForeignKey{.table_name = "Symbol",
                                                  .column_name = "id"}},
        stonks::db::Column{.name = "time",
                           .data_type = stonks::db::DataType::kInteger},
        stonks::db::Column{.name = "price",
                           .data_type = stonks::db::DataType::kReal}}};

static const auto kSymbolBookTickTableDefinition = stonks::db::TableDefinition{
    .table = stonks::db::Table{.name = "SymbolBookTick"},
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
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_DB_TABLE_DEFINITIONS_H_
