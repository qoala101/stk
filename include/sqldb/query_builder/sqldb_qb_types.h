#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_TYPES_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_TYPES_H_

#include <string>

#include "sqldb_types.h"

namespace stonks::sqldb::qb {
/**
 * @brief Tag for all columns.
 */
struct All;

/**
 * @brief Tag for selecting 1.
 */
struct One;

/**
 * @brief Alias for either column or table.
 */
template <typename Original, typename Alias>
struct As {};

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
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_TYPES_H_
