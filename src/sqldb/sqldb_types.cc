#include "sqldb_types.h"

#include <gsl/assert>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>

#include "cpp_not_null.h"

namespace stonks::sqldb {
template <cpp::DecaysTo<TableDefinition> This>
auto TableDefinition::GetColumnDefinitionImpl(This &t, const Column &column)
    -> cpp::CopyConst<This, ColumnDefinition> & {
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
    const -> cpp::ConstView<ColumnDefinition> {
  auto column_definitions =
      columns | ranges::views::transform([this](const auto &column) {
        return cpp::AssumeNn(&GetColumnDefinition(column));
      }) |
      ranges::to_vector;
  Ensures(column_definitions.size() == columns.size());
  return column_definitions;
}
}  // namespace stonks::sqldb