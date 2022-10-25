#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_TYPES_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_TYPES_H_

#include <string>

#include "sqldb_types.h"

/**
 * @file Query-builder related types.
 */

namespace stonks::sqldb::qb {
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
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_TYPES_H_
