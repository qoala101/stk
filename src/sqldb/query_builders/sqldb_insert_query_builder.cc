#include "sqldb_insert_query_builder.h"

#include <gsl/assert>
#include <type_traits>
#include <utility>
#include <variant>

#include "sqldb_query_builders_common.h"

namespace stonks::sqldb {
InsertQueryBuilder::InsertQueryBuilder(
    std::shared_ptr<IQueryBuilder> query_builder)
    : query_builder_{std::move(query_builder)} {
  Expects(query_builder_ != nullptr);
}

auto InsertQueryBuilder::IntoTable(Table table) -> InsertQueryBuilder& {
  Expects(std::holds_alternative<std::monostate>(table_));
  table_ = {std::move(table)};
  return *this;
}

auto InsertQueryBuilder::IntoTable(TableDefinition table_definition)
    -> InsertQueryBuilder& {
  Expects(std::holds_alternative<std::monostate>(table_));
  table_ = {std::move(table_definition)};
  return *this;
}

auto InsertQueryBuilder::IntoColumns(std::vector<Column> columns)
    -> InsertQueryBuilder& {
  Expects(std::holds_alternative<std::monostate>(columns_));
  columns_.emplace<std::vector<Column>>(std::move(columns));
  return *this;
}

auto InsertQueryBuilder::Build() const -> std::string {
  const auto& table = GetTable(table_);
  const auto columns_are_specified =
      !std::holds_alternative<std::monostate>(columns_);
  const auto columns = columns_are_specified
                           ? GetColumns(table_, columns_)
                           : GetColumns(table_, AllColumnsType{});
  return query_builder_->BuildInsertQuery(table, columns);
}
}  // namespace stonks::sqldb