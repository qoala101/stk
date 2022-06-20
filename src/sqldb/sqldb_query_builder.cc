#include "sqldb_query_builder.h"

#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>

#include "sqldb_types.h"

namespace stonks::sqldb {
auto IQueryBuilder::BuildSelectQuery(const TableDefinition &table_definition,
                                    const std::vector<Column> *columns) const
    -> std::string {
  return BuildSelectQuery(table_definition.table, columns);
}

auto IQueryBuilder::BuildSelectQuery(const Table &table,
                                    const std::vector<Column> *columns) const
    -> std::string {
  return BuildSelectQuery(table, columns, {});
}

auto IQueryBuilder::BuildInsertQuery(
    const TableDefinition &table_definition) const -> std::string {
  return BuildInsertQuery(
      table_definition.table,
      table_definition.columns |
          ranges::views::transform([](const auto &column_definition) {
            return column_definition.column;
          }) |
          ranges::to_vector);
}

auto IQueryBuilder::BuildUpdateQuery(const TableDefinition &table_definition,
                                    std::string_view where_clause)
    -> std::string {
  return BuildUpdateQuery(
      table_definition.table,
      table_definition.columns |
          ranges::views::transform([](const auto &column_definition) {
            return column_definition.column;
          }) |
          ranges::to_vector,
      where_clause);
}
}  // namespace stonks::sqldb