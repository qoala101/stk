#ifndef STONKS_SRC_SQLDB_SQLDB_ROW_DEFINITION_H_
#define STONKS_SRC_SQLDB_SQLDB_ROW_DEFINITION_H_
#ifndef STONKS_SQLDB_SQLDB_ROW_DEFINITION_H_
#define STONKS_SQLDB_SQLDB_ROW_DEFINITION_H_

#include <vector>

#include "cpp_views.h"
#include "sqldb_types.h"

namespace stonks::sqldb {
/**
 * @brief Types of row cells.
 */
class RowDefinition {
 public:
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  RowDefinition(std::vector<CellDefinition> cell_definitions = {});
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  RowDefinition(const TableDefinition &table_definition);
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  RowDefinition(const std::vector<ColumnDefinition> &column_definitions);
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  RowDefinition(const cpp::ConstView<ColumnDefinition> &column_definitions);

  [[nodiscard]] auto GetCellDefinitions() const
      -> const std::vector<CellDefinition> &;
  [[nodiscard]] auto GetCellDefinitions() -> std::vector<CellDefinition> &;

 private:
  std::vector<CellDefinition> cell_definitions_{};
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_ROW_DEFINITION_H_


#endif  // STONKS_SRC_SQLDB_SQLDB_ROW_DEFINITION_H_
