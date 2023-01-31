#ifndef STONKS_SQLDB_SQLDB_ROWS_H_
#define STONKS_SQLDB_SQLDB_ROWS_H_

#include <vector>

#include "cpp_this.h"  // IWYU pragma: keep
#include "sqldb_concepts.h"
#include "sqldb_types.h"
#include "sqldb_value.h"

namespace vh::sqldb {
/**
 * @brief Values for many DB rows.
 */
class Rows {
 public:
  explicit Rows(std::vector<Column> columns);

  /**
   * @brief Gives column values.
   */
  template <ColumnDefinition Column>
  auto GetColumnValues [[nodiscard]] () const -> auto & {
    return GetColumnValuesImpl<Column>(*this);
  }

  template <ColumnDefinition Column>
  auto GetColumnValues [[nodiscard]] () -> auto & {
    return GetColumnValuesImpl<Column>(*this);
  }

  /**
   * @brief Gives number of values in each column.
   */
  auto GetSize [[nodiscard]] () const -> int;

  /**
   * @brief Adds a row.
   */
  void Push(std::vector<Value> values);

 private:
  struct ColumnValues {
    Column column{};
    std::vector<Value> values{};

   private:
    friend auto operator==
        [[nodiscard]] (const ColumnValues &, const ColumnValues &)
        -> bool = default;
  };

  friend auto operator== [[nodiscard]] (const Rows &, const Rows &)
  -> bool = default;

  template <ColumnDefinition Column>
  static auto GetColumnValuesImpl [[nodiscard]] (cpp::This<Rows> auto &t)
  -> auto & {
    return t.GetColumnValues({Column::GetName()});
  }

  auto GetColumnValues [[nodiscard]] (const Column &column) const
      -> const std::vector<Value> &;
  auto GetColumnValues [[nodiscard]] (const Column &column)
  -> std::vector<Value> &;

  static auto GetColumnValuesImpl
      [[nodiscard]] (cpp::This<Rows> auto &t, const Column &column) -> auto &;

  std::vector<ColumnValues> columns_{};
};
}  // namespace vh::sqldb

#endif  // STONKS_SQLDB_SQLDB_ROWS_H_
