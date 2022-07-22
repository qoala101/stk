#include "sqldb_row.h"

#include <gsl/assert>
#include <string>
#include <utility>

namespace stonks::sqldb {
Row::Row(std::vector<Cell> cells) {
  for (auto &cell : cells) {
    cells_.emplace(std::move(cell.column), std::move(cell.value));
  }

  Ensures(cells_.size() == cells.size());
}

auto Row::GetValueImpl(auto &t, const Column &column) -> auto & {
  Expects(t.cells_.find(column) != t.cells_.end());
  return t.cells_.at(column);
}

auto Row::GetValue(const Column &column) const -> const Value & {
  return GetValueImpl(*this, column);
}

auto Row::GetValue(const Column &column) -> Value & {
  return GetValueImpl(*this, column);
}

auto Row::GetCells() const -> const std::map<Column, Value> & { return cells_; }

auto Row::GetCells() -> std::map<Column, Value> & { return cells_; }
}  // namespace stonks::sqldb