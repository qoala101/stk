#ifndef STONKS_SQLDB_SQLDB_ROWS_H_
#define STONKS_SQLDB_SQLDB_ROWS_H_

#include <vector>

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
  explicit Rows(std::vector<Column> columns);

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
   * @brief Adds a row.
   */
  void Push(std::vector<Value> values);

 private:
  struct ColumnValues {
    Column column{};
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
    return t.GetColumnValues({ColumnTraits<Column>::GetName()});
  }

  [[nodiscard]] auto GetColumnValues(const Column &column) const
      -> const std::vector<Value> &;
  [[nodiscard]] auto GetColumnValues(const Column &column)
      -> std::vector<Value> &;

  [[nodiscard]] static auto GetColumnValuesImpl(cpp::This<Rows> auto &t,
                                                const Column &column) -> auto &;

  std::vector<ColumnValues> columns_{};
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_ROWS_H_
