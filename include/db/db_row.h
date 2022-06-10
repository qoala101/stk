#ifndef STONKS_DB_DB_ROW_H_
#define STONKS_DB_DB_ROW_H_

#include <map>
#include <string>
#include <vector>

#include "db_types.h"
#include "db_value.h"

namespace stonks::db {
/**
 * @brief Single DB row.
 */
class Row {
 public:
  explicit Row(std::vector<Cell> cells);

  [[nodiscard]] auto GetValue(const Column &column) const & -> const Value &;
  [[nodiscard]] auto GetValue(const Column &column) && -> Value &&;

  [[nodiscard]] auto GetCells() const & -> const std::map<Column, Value> &;
  [[nodiscard]] auto GetCells() && -> std::map<Column, Value> &&;

 private:
  std::map<Column, Value> cells_{};
};

/**
 * @brief Many DB rows.
 */
class Rows {
 public:
  explicit Rows(std::vector<Column> columns);

  [[nodiscard]] auto GetValues(const Column &column) const
      -> const std::vector<Value> &;
  [[nodiscard]] auto GetValues(const Column &column) -> std::vector<Value> &;

  [[nodiscard]] auto GetSize() const -> int;

  void Push(Row row);
  void Push(std::vector<Value> values);

 private:
  struct ColumnValues {
    Column column{};
    std::vector<Value> values{};

   private:
    friend auto operator==(const ColumnValues &, const ColumnValues &)
        -> bool = default;
  };

  friend auto operator==(const Rows &, const Rows &) -> bool = default;

  std::vector<ColumnValues> columns_{};
};
}  // namespace stonks::db

#endif  // STONKS_DB_DB_ROW_H_
