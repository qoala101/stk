#ifndef STONKS_INCLUDE_SQLDB_SQLDB_ROW_H_
#define STONKS_INCLUDE_SQLDB_SQLDB_ROW_H_

#include <map>
#include <vector>

#include "cpp_this.h"  // IWYU pragma: keep
#include "sqldb_types.h"
#include "sqldb_value.h"

namespace stonks::sqldb {
/**
 * @brief Values for single DB row.
 */
class Row {
 public:
  explicit Row(std::vector<Cell> cells);

  [[nodiscard]] auto GetValue(const Column &column) const -> const Value &;
  [[nodiscard]] auto GetValue(const Column &column) -> Value &;

  [[nodiscard]] auto GetCells() const -> const std::map<Column, Value> &;
  [[nodiscard]] auto GetCells() -> std::map<Column, Value> &;

 private:
  [[nodiscard]] static auto GetValueImpl(cpp::This<Row> auto &t,
                                         const Column &column) -> auto &;

  [[nodiscard]] static auto GetCellsImpl(cpp::This<Row> auto &t) -> auto &;

  std::map<Column, Value> cells_{};
};
}  // namespace stonks::sqldb

#endif  // STONKS_INCLUDE_SQLDB_SQLDB_ROW_H_