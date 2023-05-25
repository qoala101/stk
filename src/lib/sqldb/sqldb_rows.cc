/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "sqldb_rows.h"

#include <gsl/assert>
#include <gsl/util>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/functional/invoke.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <utility>

#include "cpp_typed_struct.h"
#include "sqldb_types.h"

namespace vh::sqldb {
Rows::Rows(std::vector<Column> columns)
    : columns_{columns | ranges::views::transform([](auto &column) {
                 return ColumnValues{.column = std::move(column)};
               }) |
               ranges::to_vector} {
  Ensures(columns_.size() == columns.size());
}

auto Rows::GetSize() const -> int {
  if (columns_.empty()) {
    return 0;
  }

  return gsl::narrow_cast<int>(columns_.front().values.size());
}

void Rows::Push(std::vector<Value> values) {
  Expects(values.size() == columns_.size());

  for (auto i = 0; i < gsl::narrow_cast<int>(values.size()); ++i) {
    columns_[i].values.emplace_back(std::move(values[i]));
  }
}

auto Rows::GetColumnValuesImpl(cpp::This<Rows> auto &t, const Column &column)
    -> auto & {
  const auto iter =
      ranges::find_if(t.columns_, [column](const auto &column_values) {
        return column_values.column == column;
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
}  // namespace vh::sqldb