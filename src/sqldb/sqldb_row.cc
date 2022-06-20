#include "sqldb_row.h"

#include <absl/base/macros.h>

#include <gsl/assert>
#include <string>
#include <utility>

namespace stonks::sqldb {
Row::Row(std::vector<Cell> cells) {
  for (auto &cell : cells) {
    cells_.emplace(std::move(cell.column), std::move(cell.value));
  }
}

auto Row::GetValue(const Column &column) const -> const Value & {
  Expects(cells_.find(column) != cells_.end());
  return cells_.at(column);
}

auto Row::GetValue(const Column &column) -> Value & {
  // NOLINTNEXTLINE(*-const-cast)
  return const_cast<Value &>(const_cast<const Row *>(this)->GetValue(column));
}

auto Row::GetCells() const -> const std::map<Column, Value> & { return cells_; }

auto Row::GetCells() -> std::map<Column, Value> & {
  // NOLINTNEXTLINE(*-const-cast)
  return const_cast<std::map<Column, Value> &>(
      // NOLINTNEXTLINE(*-const-cast)
      const_cast<const Row *>(this)->GetCells());
}
}  // namespace stonks::sqldb