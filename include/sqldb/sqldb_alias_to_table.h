#ifndef STONKS_SQLDB_SQLDB_ALIAS_TO_TABLE_H_
#define STONKS_SQLDB_SQLDB_ALIAS_TO_TABLE_H_

#include "sqldb_table.h"

namespace stonks::sqldb {
/**
 * @brief Base for table alias definition types.
 */
template <typename TargetTable, typename AliasTable>
struct AliasToTable : public Table<AliasTable> {
  /**
   * @brief Target table used to refer its columns in alias table.
   */
  using Target = TargetTable;

  /**
   * @brief Base for column alias which allows to override the type.
   */
  template <typename TargetColumn, typename AliasColumn = TargetColumn>
  struct AliasToColumn : public TargetColumn {
    /**
     * @copydoc Column::GetName
     */
    [[nodiscard]] static auto GetName() { return cpp::NameOf<AliasColumn>(); }

    /**
     * @copydoc Column::GetFullName
     */
    [[nodiscard]] static auto GetFullName() {
      return TargetColumn::template GetFullNameInTable<AliasTable>();
    }
  };
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_ALIAS_TO_TABLE_H_
