#ifndef STONKS_SQLDB_SQLDB_ROWS_H_
#define STONKS_SQLDB_SQLDB_ROWS_H_

#include <bits/ranges_algo.h>

#include <range/v3/algorithm/find_if.hpp>
#include <vector>

#include "cpp_copy_const.h"
#include "cpp_this.h"  // IWYU pragma: keep
#include "sqldb_traits.h"
#include "sqldb_types.h"
#include "sqldb_value.h"

namespace stonks::sqldb {
/**
 * @brief Values for many DB rows.
 */
class Rows {
 public:
  explicit Rows(std::vector<std::string> column_names = {});

  /**
   * @brief Gives column values.
   */
  template <typename Column>
  [[nodiscard]] auto GetColumnValues() const -> auto & {
    return GetColumnValuesImpl<Column>(*this);
  }

  template <typename Column>
  [[nodiscard]] auto GetColumnValues() -> auto & {
    return GetColumnValuesImpl<Column>(*this);
  }

  /**
   * @brief Gives number of values in each column.
   */
  [[nodiscard]] auto GetSize() const -> int;

  /**
   * @brief Tells whether columns are empty.
   */
  [[nodiscard]] auto IsEmpty() const -> bool;

  /**
   * @brief Adds a row.
   */
  void Push(std::vector<Value> values);

 private:
  struct ColumnValues {
    std::string column_name{};
    std::vector<Value> values{};

   private:
    [[nodiscard]] friend auto operator==(const ColumnValues &,
                                         const ColumnValues &)
        -> bool = default;
  };

  [[nodiscard]] friend auto operator==(const Rows &, const Rows &)
      -> bool = default;

  template <typename Column>
  [[nodiscard]] static auto GetColumnValuesImpl(cpp::This<Rows> auto &t)
      -> auto & {
    return t.GetColumnValues(ColumnTraits<Column>::GetName());
  }

  [[nodiscard]] auto GetColumnValues(std::string_view column_name) const
      -> const std::vector<Value> &;
  [[nodiscard]] auto GetColumnValues(std::string_view column_name)
      -> std::vector<Value> &;

  [[nodiscard]] static auto GetColumnValuesImpl(cpp::This<Rows> auto &t,
                                                std::string_view column_name)
      -> auto &;

  std::vector<ColumnValues> columns_{};
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_ROWS_H_
