#ifndef STONKS_SQLDB_SQLDB_ROW_H_
#define STONKS_SQLDB_SQLDB_ROW_H_

#include <map>
#include <vector>

#include "cpp_copy_const.h"
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
  template <cpp::DecaysTo<Row> This>
  [[nodiscard]] static auto GetValueImpl(This &t, const Column &column)
      -> cpp::CopyConst<This, Value &>;

  template <cpp::DecaysTo<Row> This>
  [[nodiscard]] static auto GetCellsImpl(This &t)
      -> cpp::CopyConst<This, std::map<Column, Value> &>;

  std::map<Column, Value> cells_{};
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_ROW_H_
