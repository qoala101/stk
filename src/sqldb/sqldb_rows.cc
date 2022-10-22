#include "sqldb_rows.h"

#include <gsl/assert>
#include <gsl/util>
#include <memory>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <utility>

#include "cpp_typed_struct.h"

namespace stonks::sqldb {
Rows::Rows(std::vector<std::string> column_names)
    : columns_{column_names |
               ranges::views::transform([](std::string &column_name) {
                 return ColumnValues{std::move(column_name)};
               }) |
               ranges::to_vector} {
  Ensures(columns_.size() == column_names.size());
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

auto Rows::GetColumnValuesImpl(cpp::This<Rows> auto &t,
                               std::string_view column_name) -> auto & {
  const auto iter = ranges::find_if(
      t.columns_, [column_name](const ColumnValues &other_column) {
        return other_column.column_name == column_name;
      });
  Expects(iter != t.columns_.end());
  return iter->values;
}

auto Rows::GetColumnValues(std::string_view column_name) const
    -> const std::vector<Value> & {
  return GetColumnValuesImpl(*this, column_name);
}
auto Rows::GetColumnValues(std::string_view column_name)
    -> std::vector<Value> & {
  return GetColumnValuesImpl(*this, column_name);
}
}  // namespace stonks::sqldb