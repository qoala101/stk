#ifndef STONKS_DB_DB_ROW_DEFINITION_H_
#define STONKS_DB_DB_ROW_DEFINITION_H_

#include <vector>

#include "db_types.h"

namespace stonks::db {
/**
 * @brief Types of row cells.
 */
class RowDefinition {
 public:
  RowDefinition() = default;
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  RowDefinition(const TableDefinition &table_definition);
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  RowDefinition(std::vector<CellDefinition> cell_definitions);
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  RowDefinition(const std::vector<ColumnDefinition> &column_definitions);

  [[nodiscard]] auto GetCellDefinitions() const
      -> const std::vector<CellDefinition> &;

 private:
  std::vector<CellDefinition> cell_definitions_{};
};
}  // namespace stonks::db

#endif  // STONKS_DB_DB_ROW_DEFINITION_H_
