#include "sqldb_qbf_common.h"

#include <gsl/assert>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <variant>
#include <vector>

#include "sqldb_types.h"

namespace stonks::sqldb::qbf {
auto GetColumns(const TableVariant &table, const ColumnsVariant &columns)
    -> std::vector<Column> {
  Expects(columns.HasColumns());

  if (const auto columns_are_specified =
          std::holds_alternative<std::vector<Column>>(columns.value)) {
    return std::get<std::vector<Column>>(columns.value);
  }

  Expects(std::holds_alternative<TableDefinition>(table));
  const auto &table_definition = std::get<TableDefinition>(table);

  auto table_columns =
      table_definition.columns |
      ranges::views::transform([](const auto &column_definition) {
        return column_definition.column;
      }) |
      ranges::to_vector;
  Expects(table_columns.size() == table_definition.columns.size());
  return table_columns;
}

auto GetColumns(const cpp::ConstView<ColumnDefinition> &column_definitions)
    -> std::vector<Column> {
  auto columns = column_definitions |
                 ranges::views::transform([](const auto &column_definition) {
                   return column_definition->column;
                 }) |
                 ranges::to_vector;
  Expects(columns.size() == column_definitions.size());
  return columns;
}
}  // namespace stonks::sqldb::qbf
