#include "sqldb_types.h"

#include <gsl/assert>
#include <range/v3/algorithm/find_if.hpp>

namespace stonks::sqldb {
auto TableDefinition::GetColumnDefinition(const Column &column) const
    -> const ColumnDefinition & {
  const auto iter =
      ranges::find_if(columns, [&column](const auto &column_definition) {
        return column_definition.column == column;
      });
  Expects(iter != columns.end());
  return *iter.base();
}
}  // namespace stonks::sqldb