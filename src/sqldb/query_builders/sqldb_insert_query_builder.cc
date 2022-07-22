#include "sqldb_insert_query_builder.h"

#include <gsl/assert>
#include <type_traits>
#include <utility>
#include <variant>

#include "sqldb_query_builders_common.h"

namespace stonks::sqldb::query_builder_facade {
InsertQueryBuilder::InsertQueryBuilder(
    cpp::not_null<std::shared_ptr<IQueryBuilder>> query_builder)
    : query_builder_{std::move(query_builder)} {}

auto InsertQueryBuilder::WholeRow() -> InsertQueryBuilder& {
  Expects(std::holds_alternative<std::monostate>(columns_));
  columns_.emplace<AllColumnsType>();
  Ensures(!std::holds_alternative<std::monostate>(columns_));
  return *this;
}

auto InsertQueryBuilder::IntoTable(Table table) -> InsertQueryBuilder& {
  Expects(std::holds_alternative<std::monostate>(table_));
  table_ = {std::move(table)};
  Ensures(!std::holds_alternative<std::monostate>(table_));
  return *this;
}

auto InsertQueryBuilder::IntoTable(TableDefinition table_definition)
    -> InsertQueryBuilder& {
  Expects(std::holds_alternative<std::monostate>(table_));
  table_ = {std::move(table_definition)};
  Ensures(!std::holds_alternative<std::monostate>(table_));
  return *this;
}

auto InsertQueryBuilder::IntoColumns(std::vector<Column> columns)
    -> InsertQueryBuilder& {
  Expects(std::holds_alternative<std::monostate>(columns_));
  columns_.emplace<std::vector<Column>>(std::move(columns));
  Ensures(!std::holds_alternative<std::monostate>(columns_));
  return *this;
}

auto InsertQueryBuilder::IntoColumns(
    const ccutils::ConstView<ColumnDefinition>& column_definitions)
    -> InsertQueryBuilder& {
  return IntoColumns(GetColumns(column_definitions));
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
}  // namespace stonks::sqldb::query_builder_facade