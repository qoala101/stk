#ifndef STONKS_SQLDB_SQLDB_ALIAS_TO_TABLE_H_
#define STONKS_SQLDB_SQLDB_ALIAS_TO_TABLE_H_

#include "cpp_type_list.h"
#include "sqldb_concepts.h"
#include "sqldb_table.h"

namespace stonks::sqldb {
/**
 * @brief Base for table alias definition types.
 */
template <TableDefinition TargetTable, typename AliasTable>
struct AliasToTable : public Table<AliasTable> {
  /**
   * @brief Target table used to refer its columns in alias table.
   */
  using Target = TargetTable;

  /**
   * @brief Alias table has no columns.
   */
  using Columns = cpp::TypeList<>;

  /**
   * @brief Base for column alias which allows to override the type.
   */
  template <ColumnDefinition TargetColumn, typename AliasColumn = TargetColumn>
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
