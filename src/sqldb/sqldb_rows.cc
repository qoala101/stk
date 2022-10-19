#include "sqldb_rows.h"

#include <gsl/assert>
#include <gsl/util>
#include <memory>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/transform.hpp>
#include <string>
#include <utility>

namespace stonks::sqldb {
Rows::Rows(std::vector<Column> columns)
    : columns_{columns | ranges::views::transform([](Column &column) {
                 return ColumnValues{std::move(column)};
               }) |
               ranges::to_vector} {
  Ensures(columns_.size() == columns.size());
}

auto Rows::GetColumnValuesImpl(cpp::This<Rows> auto &t, const Column &column)
    -> auto & {
  const auto iter =
      ranges::find_if(t.columns_, [&column](const ColumnValues &other_column) {
        return other_column.column == column;
      });
  Expects(iter != t.columns_.end());
  return iter->values;
}

auto Rows::GetColumnValues(const Column &column) const
    -> const std::vector<Value> & {
  return GetColumnValuesImpl(*this, column);
}

auto Rows::GetColumnValues(const Column &column) -> std::vector<Value> & {
  return GetColumnValuesImpl(*this, column);
}

auto Rows::GetCellValueImpl(cpp::This<Rows> auto &t, const Column &column,
                            int row_index) -> auto & {
  Expects(row_index >= 0);

  auto &values = t.GetColumnValues(column);
  Expects(row_index < gsl::narrow_cast<int>(values.size()));

  return values[row_index];
}

auto Rows::GetCellValue(const Column &column, int row_index) const
    -> const Value & {
  return GetCellValueImpl(*this, column, row_index);
}

auto Rows::GetCellValue(const Column &column, int row_index) -> Value & {
  return GetCellValueImpl(*this, column, row_index);
}

auto Rows::GetFirstImpl(cpp::This<Rows> auto &t, const Column &column)
    -> auto & {
  return t.GetCellValue(column, 0);
}

auto Rows::GetFirst(const Column &column) const -> const Value & {
  return GetFirstImpl(*this, column);
}

auto Rows::GetFirst(const Column &column) -> Value & {
  return GetFirstImpl(*this, column);
}

auto Rows::GetSize() const -> int {
  if (columns_.empty()) {
    return 0;
  }

  return gsl::narrow_cast<int>(columns_.front().values.size());
}

auto Rows::IsEmpty() const -> bool { return GetSize() == 0; }

void Rows::Push(std::vector<Value> values) {
  Expects(values.size() == columns_.size());

  for (auto i = 0; i < gsl::narrow_cast<int>(values.size()); ++i) {
    columns_[i].values.emplace_back(std::move(values[i]));
  }
}
}  // namespace stonks::sqldb