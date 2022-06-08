#include "db_row.h"

#include <absl/base/macros.h>

#include <memory>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/transform.hpp>
#include <type_traits>
#include <utility>

namespace stonks::db {
Row::Row(std::vector<Cell> cells) {
  for (auto &cell : cells) {
    cells_.emplace(std::move(cell.column), std::move(cell.value));
  }
}

auto Row::GetValue(const Column &column) const & -> const Value & {
  ABSL_ASSERT((cells_.find(column) != cells_.end()) && "No cell in row");
  return cells_.at(column);
}

auto Row::GetValue(const Column &column) && -> Value && {
  return std::move(
      // NOLINTNEXTLINE(*-const-cast)
      const_cast<Value &>(const_cast<const Row *>(this)->GetValue(column)));
}

auto Row::GetCells() const & -> const std::map<Column, Value> & {
  return cells_;
}

auto Row::GetCells() && -> std::map<Column, Value> && {
  return std::move(
      // NOLINTNEXTLINE(*-const-cast)
      const_cast<std::map<Column, Value> &>(
          // NOLINTNEXTLINE(*-const-cast)
          const_cast<const Row *>(this)->GetCells()));
}

Rows::Rows(std::vector<Column> columns)
    : columns_{
          ranges::views::transform(
              columns,
              [](auto &column) { return ColumnValues{std::move(column)}; }) |
          ranges::to_vector} {}

[[nodiscard]] auto Rows::GetValues(const Column &column) const
    -> const std::vector<Value> & {
  const auto iter =
      ranges::find_if(columns_, [&column](const auto &other_column) {
        return other_column.column == column;
      });
  ABSL_ASSERT((iter != columns_.end()) &&
              "Requesting values for non existent column");
  return iter->values;
}

[[nodiscard]] auto Rows::GetValues(const Column &column)
    -> std::vector<Value> & {
  // NOLINTNEXTLINE(*-const-cast)
  return const_cast<std::vector<Value> &>(
      // NOLINTNEXTLINE(*-const-cast)
      const_cast<const Rows *>(this)->GetValues(column));
}

[[nodiscard]] auto Rows::GetSize() const -> int {
  if (columns_.empty()) {
    return 0;
  }

  return static_cast<int>(columns_.front().values.size());
}

void Rows::Push(Row row) {
  for (auto &column : columns_) {
    column.values.emplace_back(std::move(row).GetValue(column.column));
  }
}

void Rows::Push(std::vector<Value> values) {
  ABSL_ASSERT((values.size() == columns_.size()) &&
              "Pushing wrong amount of values");

  for (auto i = 0; i < values.size(); ++i) {
    columns_[i].values.emplace_back(std::move(values[i]));
  }
}
}  // namespace stonks::db