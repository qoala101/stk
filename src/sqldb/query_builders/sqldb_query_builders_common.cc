#include "sqldb_query_builders_common.h"

#include <gsl/assert>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <type_traits>
#include <variant>
#include <vector>

#include "sqldb_types.h"

namespace stonks::sqldb::query_builder_facade {
auto GetTable(const TableVariant &table) -> Table {
  return std::visit(
      [](const auto &variant) -> const Table & {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, Table>) {
          return variant;
        }

        if constexpr (std::is_same_v<T, TableDefinition>) {
          return variant.table;
        }

        Expects(false);
      },
      table);
}

auto GetColumns(const TableVariant &table, const ColumnsVariant &columns)
    -> std::vector<Column> {
  Expects(!std::holds_alternative<std::monostate>(columns));

  if (const auto columns_are_specified =
          std::holds_alternative<std::vector<Column>>(columns)) {
    return std::get<std::vector<Column>>(columns);
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

auto GetColumns(const ccutils::ConstView<ColumnDefinition> &column_definitions)
    -> std::vector<Column> {
  auto columns = column_definitions |
                 ranges::views::transform([](const auto &column_definition) {
                   return column_definition->column;
                 }) |
                 ranges::to_vector;
  Expects(columns.size() == column_definitions.size());
  return columns;
}
}  // namespace stonks::sqldb::query_builder_facade
