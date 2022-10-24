#include "sqldb_row_definition.h"

#include <gsl/assert>
#include <memory>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <utility>

#include "cpp_not_null.h"
#include "not_null.hpp"
#include "sqldb_types.h"

namespace stonks::sqldb {
RowDefinition::RowDefinition(const TableDefinition &table_definition)
    : RowDefinition{table_definition.columns} {}

RowDefinition::RowDefinition(std::vector<CellDefinition> cell_definitions)
    : cell_definitions_{std::move(cell_definitions)} {}

RowDefinition::RowDefinition(
    const std::vector<ColumnDefinition> &column_definitions)
    : RowDefinition{column_definitions |
                    ranges::views::transform(
                        [](const ColumnDefinition &column_definition) {
                          return CellDefinition{column_definition.column,
                                                column_definition.data_type};
                        }) |
                    ranges::to_vector} {
  Ensures(cell_definitions_.size() == column_definitions.size());
}

RowDefinition::RowDefinition(
    const cpp::ConstView<ColumnDefinition> &column_definitions)
    : RowDefinition{
          column_definitions |
          ranges::views::transform(
              [](cpp::Nn<const ColumnDefinition *> column_definition) {
                return CellDefinition{column_definition->column,
                                      column_definition->data_type};
              }) |
          ranges::to_vector} {
  Ensures(cell_definitions_.size() == column_definitions.size());
}

auto RowDefinition::GetCellDefinitions() const
    -> const std::vector<CellDefinition> & {
  return cell_definitions_;
}

auto RowDefinition::GetCellDefinitions() -> std::vector<CellDefinition> & {
  return cell_definitions_;
}
}  // namespace stonks::sqldb