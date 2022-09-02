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

auto Row::GetValueImpl(cpp::DecaysTo<Row> auto &&t, const Column &column)
    -> cpp::DecaysTo<Value> auto && {
  auto &&ft = std::forward<decltype(t)>(t);
  Expects(ft.cells_.find(column) != ft.cells_.end());
  return ft.cells_.at(column);
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