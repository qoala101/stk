#include "sqldb_types.h"

#include <gsl/assert>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>

#include "not_null.hpp"

namespace stonks::sqldb {
auto TableDefinition::GetColumnDefinitionImpl(auto &t, const Column &column)
    -> auto & {
  const auto iter =
      ranges::find_if(t.columns, [&column](const auto &column_definition) {
        return column_definition.column == column;
      });
  Expects(iter != t.columns.end());
  return *iter.base();
}

auto TableDefinition::GetColumnDefinition(const Column &column) const
    -> const ColumnDefinition & {
  return GetColumnDefinitionImpl(*this, column);
}

auto TableDefinition::GetColumnDefinition(const Column &column)
    -> ColumnDefinition & {
  return GetColumnDefinitionImpl(*this, column);
}

auto TableDefinition::GetColumnDefinitions(const std::vector<Column> &columns)
    const -> ccutils::ConstView<ColumnDefinition> {
  auto column_definitions =
      columns | ranges::views::transform([this](const auto &column) {
        return cpp::assume_not_null(&GetColumnDefinition(column));
      }) |
      ranges::to_vector;
  Ensures(column_definitions.size() == columns.size());
  return column_definitions;
}
}  // namespace stonks::sqldb