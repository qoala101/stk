#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_TYPES_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_TYPES_H_

#include <string>

#include "sqldb_types.h"

/**
 * @file Query-builder related types.
 */

namespace stonks::sqldb::qb {
/**
 * @brief Tag for all columns.
 */
struct All;

/**
 * @brief Tag for selecting 1.
 */
struct One;

struct ColumnDefinition {
  std::string name{};
  DataTypeVariant type{};
  bool unique{};
};

struct PrimaryKey {
  std::string column_name{};
  bool auto_increment{};
};

struct ForeignKey {
  std::string column_name{};
  std::string target_table_name{};
  std::string target_column_name{};
};

struct FullColumnType {
  std::string name{};
  std::string full_name{};
  DataTypeVariant type{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_TYPES_H_
