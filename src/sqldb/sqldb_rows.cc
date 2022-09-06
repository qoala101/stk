#include "sqldb_rows.h"

#include <gsl/assert>
#include <gsl/util>
#include <memory>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/transform.hpp>
#include <utility>

namespace stonks::sqldb {
Rows::Rows(std::vector<Column> columns)
    : columns_{
          ranges::views::transform(
              columns,
              [](auto &column) { return ColumnValues{std::move(column)}; }) |
          ranges::to_vector} {
  Ensures(columns_.size() == columns.size());
}

template <cpp::DecaysTo<Rows> This>
auto Rows::GetColumnValuesImpl(This &t, const Column &column)
    -> cpp::CopyConst<This, std::vector<Value> &> {
  const auto iter =
      ranges::find_if(t.columns_, [&column](const auto &other_column) {
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

auto Rows::GetSize() const -> int {
  if (columns_.empty()) {
    return 0;
  }

  return gsl::narrow_cast<int>(columns_.front().values.size());
}

void Rows::Push(std::vector<Value> values) {
  Expects(values.size() == columns_.size());

  for (auto i = 0; i < values.size(); ++i) {
    columns_[i].values.emplace_back(std::move(values[i]));
  }
}
}  // namespace stonks::sqldb