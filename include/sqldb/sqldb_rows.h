#ifndef STONKS_SQLDB_SQLDB_ROWS_H_
#define STONKS_SQLDB_SQLDB_ROWS_H_

#include <vector>

#include "cpp_this.h"  // IWYU pragma: keep
#include "sqldb_types.h"
#include "sqldb_value.h"

namespace stonks::sqldb {
/**
 * @brief Values for many DB rows.
 */
class Rows {
 public:
  explicit Rows(std::vector<Column> columns = {});

  /**
   * @brief Gives column values.
   */
  [[nodiscard]] auto GetColumnValues(const Column &column) const
      -> const std::vector<Value> &;
  [[nodiscard]] auto GetColumnValues(const Column &column)
      -> std::vector<Value> &;

  /**
   * @brief Gives column cell value.
   */
  [[nodiscard]] auto GetCellValue(const Column &column, int row_index) const
      -> const Value &;
  [[nodiscard]] auto GetCellValue(const Column &column, int row_index)
      -> Value &;

  /**
   * @brief Gives cell value of the first row.
   */
  [[nodiscard]] auto GetFirst(const Column &column) const -> const Value &;
  [[nodiscard]] auto GetFirst(const Column &column) -> Value &;

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
    Column column{};
    std::vector<Value> values{};

   private:
    [[nodiscard]] friend auto operator==(const ColumnValues &,
                                         const ColumnValues &)
        -> bool = default;
  };

  [[nodiscard]] friend auto operator==(const Rows &, const Rows &)
      -> bool = default;

  [[nodiscard]] static auto GetColumnValuesImpl(cpp::This<Rows> auto &t,
                                                const Column &column) -> auto &;

  [[nodiscard]] static auto GetCellValueImpl(cpp::This<Rows> auto &t,
                                             const Column &column,
                                             int row_index) -> auto &;

  [[nodiscard]] static auto GetFirstImpl(cpp::This<Rows> auto &t,
                                         const Column &column) -> auto &;

  std::vector<ColumnValues> columns_{};
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_ROWS_H_
