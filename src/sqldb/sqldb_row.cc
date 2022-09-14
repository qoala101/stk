#include "sqldb_row.h"

#include <gsl/assert>
#include <utility>

#include "cpp_typed_struct.h"

namespace stonks::sqldb {
Row::Row(std::vector<Cell> cells) {
  for (auto &cell : cells) {
    cells_.emplace(std::move(cell.column), std::move(cell.value));
  }

  Ensures(cells_.size() == cells.size());
}

template <cpp::DecaysTo<Row> This>
auto Row::GetValueImpl(This &t, const Column &column)
    -> cpp::CopyConst<This, Value &> {
  Expects(t.cells_.find(column) != t.cells_.end());
  return t.cells_.at(column);
}

auto Row::GetValue(const Column &column) const -> const Value & {
  return GetValueImpl(*this, column);
}

auto Row::GetValue(const Column &column) -> Value & {
  return GetValueImpl(*this, column);
}

template <cpp::DecaysTo<Row> This>
auto Row::GetCellsImpl(This &t)
    -> cpp::CopyConst<This, std::map<Column, Value> &> {
  return t.cells_;
}

auto Row::GetCells() const -> const std::map<Column, Value> & {
  return GetCellsImpl(*this);
}

auto Row::GetCells() -> std::map<Column, Value> & {
  return GetCellsImpl(*this);
}
}  // namespace stonks::sqldb