#include "app_sdb_tables.h"

#include <string>

#include "cpp_typed_struct.h"
#include "sqldb_enums.h"

namespace stonks::app::sdb::tables {
auto Asset::Definition() -> const sqldb::TableDefinition& {
  static const auto kDefinition =
      sqldb::TableDefinition{.table = {kTable},
                             .columns = {{.column = {kId},
                                          .data_type = sqldb::DataType::kInt64,
                                          .primary_key = true,
                                          .auto_increment = true,
                                          .unique = true},
                                         {.column = {kName},
                                          .data_type = sqldb::DataType::kString,
                                          .unique = true}}};
  return kDefinition;
}

auto SymbolInfo::Definition() -> const sqldb::TableDefinition& {
  static const auto kDefinition = sqldb::TableDefinition{
      .table = {kTable},
      .columns = {
          {.column = {kId},
           .data_type = sqldb::DataType::kInt64,
           .primary_key = true,
           .auto_increment = true,
           .unique = true},
          {.column = {kName},
           .data_type = sqldb::DataType::kString,
           .unique = true},
          {.column = {kBaseAssetId},
           .data_type = sqldb::DataType::kInt64,
           .foreign_key = {{.table = {Asset::kTable}, .column = {Asset::kId}}}},
          {.column = {kBaseAssetMinAmount},
           .data_type = sqldb::DataType::kDouble},
          {.column = {kBaseAssetPriceStep},
           .data_type = sqldb::DataType::kDouble},
          {.column = {kQuoteAssetId},
           .data_type = sqldb::DataType::kInt64,
           .foreign_key = {{.table = {Asset::kTable}, .column = {Asset::kId}}}},
          {.column = {kQuoteAssetMinAmount},
           .data_type = sqldb::DataType::kDouble},
          {.column = {kQuoteAssetPriceStep},
           .data_type = sqldb::DataType::kDouble}}};
  return kDefinition;
}

auto SymbolPriceRecord::Definition() -> const sqldb::TableDefinition& {
  static const auto kDefinition = sqldb::TableDefinition{
      .table = {kTable},
      .columns = {{.column = {kSymbolId},
                   .data_type = sqldb::DataType::kInt64,
                   .foreign_key = {{.table = {SymbolInfo::kTable},
                                    .column = {SymbolInfo::kId}}}},
                  {.column = {kPrice}, .data_type = sqldb::DataType::kDouble},
                  {.column = {kTime}, .data_type = sqldb::DataType::kInt64}}};
  return kDefinition;
}
}  // namespace stonks::app::sdb::tables