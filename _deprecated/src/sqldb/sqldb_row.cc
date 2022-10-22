#include "sqldb_row.h"

#include <gsl/assert>
#include <string>
#include <utility>

#include "cpp_typed_struct.h"

namespace stonks::sqldb {
Row::Row(std::vector<Cell> cells) {
  for (auto &cell : cells) {
    cells_.emplace(std::move(cell.column), std::move(cell.value));
  }

  Ensures(cells_.size() == cells.size());
}

auto Row::GetValueImpl(cpp::This<Row> auto &t, const Column &column) -> auto & {
  Expects(t.cells_.find(column) != t.cells_.end());
  return t.cells_.at(column);
}

auto Row::GetValue(const Column &column) const -> const Value & {
  return GetValueImpl(*this, column);
}

auto Row::GetValue(const Column &column) -> Value & {
  return GetValueImpl(*this, column);
}

auto Row::GetCellsImpl(cpp::This<Row> auto &t) -> auto & { return t.cells_; }

auto Row::GetCells() const -> const std::map<Column, Value> & {
  return GetCellsImpl(*this);
}

auto Row::GetCells() -> std::map<Column, Value> & {
  return GetCellsImpl(*this);
}
}  // namespace stonks::sqldb