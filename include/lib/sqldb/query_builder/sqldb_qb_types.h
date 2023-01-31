#ifndef VH_SQLDB_QB_TYPES_H_
#define VH_SQLDB_QB_TYPES_H_

#include <string>
#include <string_view>

#include "sqldb_concepts.h"  // IWYU pragma: keep
#include "sqldb_types.h"

namespace vh::sqldb::qb {
namespace detail {
auto GetFullAliasName
    [[nodiscard]] (std::string_view target_name, std::string_view alias_name)
    -> std::string;
}  // namespace detail

/**
 * @brief Order for select statements.
 */
enum class Order { kAscending, kDescending };

/**
 * @brief Tag for all columns.
 */
struct All {};

/**
 * @brief Tag for selecting 1.
 */
struct One {};

/**
 * @brief Tag for selecting unique values.
 */
struct Distinct {};

template <typename Target, typename Alias>
struct As;

/**
 * @brief Alias for table.
 */
template <TableDefinition TargetTable, TableDefinition AliasTable>
struct As<TargetTable, AliasTable> : public TargetTable {
  /**
   * @copydoc Table::GetName
   */
  static auto GetName [[nodiscard]] () {
    return detail::GetFullAliasName(TargetTable::GetName(),
                                    AliasTable::GetName());
  }
};

/**
 * @brief Alias for column.
 */
template <ColumnDefinition TargetColumn, ColumnDefinition AliasColumn>
struct As<TargetColumn, AliasColumn> : public TargetColumn {
  /**
   * @copydoc Table::Column::GetName
   */
  static auto GetName [[nodiscard]] () { return AliasColumn::GetName(); }

  /**
   * @copydoc Table::Column::GetFullName
   */
  static auto GetFullName [[nodiscard]] () {
    return detail::GetFullAliasName(TargetColumn::GetFullName(),
                                    AliasColumn::GetName());
  }
};

/**
 * @brief Data required to create column.
 */
struct CreateColumnData {
  std::string name{};
  DataTypeVariant type{};
  bool unique{};
};

/**
 * @brief Data required to create primary key.
 */
struct PrimaryKeyData {
  std::string column_name{};
  bool auto_increment{};
};

/**
 * @brief Data required to create foreign key.
 */
struct ForeignKeyData {
  std::string column_name{};
  std::string target_table_name{};
  std::string target_column_name{};
};

/**
 * @brief Data required to select values from column.
 */
struct SelectColumnData {
  std::string name{};
  std::string full_name{};
  DataTypeVariant type{};
};
}  // namespace vh::sqldb::qb

#endif  // VH_SQLDB_QB_TYPES_H_
