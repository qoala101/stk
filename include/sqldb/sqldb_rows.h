#ifndef STONKS_SQLDB_SQLDB_ROWS_H_
#define STONKS_SQLDB_SQLDB_ROWS_H_

#include <vector>

#include "sqldb_types.h"
#include "sqldb_value.h"

namespace stonks::sqldb {
/**
 * @brief Values for many DB rows.
 */
class Rows {
 public:
  explicit Rows(std::vector<Column> columns = {});

  [[nodiscard]] auto GetColumnValues(const Column &column) const
      -> const std::vector<Value> &;
  [[nodiscard]] auto GetColumnValues(const Column &column)
      -> std::vector<Value> &;

  [[nodiscard]] auto GetSize() const -> int;

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

  template <cpp::DecaysTo<Rows> This>
  [[nodiscard]] static auto GetColumnValuesImpl(This &t, const Column &column)
      -> auto &;

  std::vector<ColumnValues> columns_{};
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_ROWS_H_
