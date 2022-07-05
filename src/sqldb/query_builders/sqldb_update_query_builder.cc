#include "sqldb_update_query_builder.h"

#include <gsl/assert>
#include <type_traits>
#include <utility>
#include <variant>

#include "sqldb_query_builders_common.h"

namespace stonks::sqldb {
UpdateQueryBuilder::UpdateQueryBuilder(
    cpp::not_null<std::shared_ptr<IQueryBuilder>> query_builder)
    : query_builder_{std::move(query_builder)} {}

auto UpdateQueryBuilder::OfTable(Table table) -> UpdateQueryBuilder& {
  Expects(std::holds_alternative<std::monostate>(table_));
  table_ = {std::move(table)};
  return *this;
}

auto UpdateQueryBuilder::OfTable(TableDefinition table_definition)
    -> UpdateQueryBuilder& {
  Expects(std::holds_alternative<std::monostate>(table_));
  table_ = {std::move(table_definition)};
  return *this;
}

auto UpdateQueryBuilder::Columns(std::vector<Column> columns)
    -> UpdateQueryBuilder& {
  Expects(std::holds_alternative<std::monostate>(columns_));
  columns_.emplace<std::vector<Column>>(std::move(columns));
  return *this;
}

auto UpdateQueryBuilder::Columns(
    const ConstView<ColumnDefinition>& column_definitions)
    -> UpdateQueryBuilder& {
  return Columns(GetColumns(column_definitions));
}

auto UpdateQueryBuilder::AllColumns() -> UpdateQueryBuilder& {
  Expects(std::holds_alternative<std::monostate>(columns_));
  columns_.emplace<AllColumnsType>();
  return *this;
}

auto UpdateQueryBuilder::Where(std::string_view where_clause)
    -> UpdateQueryBuilder& {
  Expects(!where_clause_.has_value());
  where_clause_.emplace(std::string{"WHERE "} + where_clause.data());
  return *this;
}

auto UpdateQueryBuilder::Build() const -> std::string {
  const auto& table = GetTable(table_);
  const auto columns = GetColumns(table_, columns_);
  return query_builder_->BuildUpdateQuery(
      table, columns, where_clause_.value_or(std::string{}));
}
}  // namespace stonks::sqldb