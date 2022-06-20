#include "sqldb_rows.h"

#include <absl/base/macros.h>

#include <gsl/assert>
#include <memory>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/functional/identity.hpp>
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
          ranges::to_vector} {}

auto Rows::GetValues(const Column &column) const -> const std::vector<Value> & {
  const auto iter =
      ranges::find_if(columns_, [&column](const auto &other_column) {
        return other_column.column == column;
      });
  Expects(iter != columns_.end());
  return iter->values;
}

auto Rows::GetValues(const Column &column) -> std::vector<Value> & {
  // NOLINTNEXTLINE(*-const-cast)
  return const_cast<std::vector<Value> &>(
      // NOLINTNEXTLINE(*-const-cast)
      const_cast<const Rows *>(this)->GetValues(column));
}

auto Rows::GetSize() const -> int {
  if (columns_.empty()) {
    return 0;
  }

  return static_cast<int>(columns_.front().values.size());
}

void Rows::Push(std::vector<Value> values) {
  Expects(values.size() == columns_.size());

  for (auto i = 0; i < values.size(); ++i) {
    columns_[i].values.emplace_back(std::move(values[i]));
  }
}
}  // namespace stonks::sqldb