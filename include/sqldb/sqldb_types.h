#ifndef STONKS_SQLDB_SQLDB_TYPES_H_
#define STONKS_SQLDB_SQLDB_TYPES_H_

#include <string>
#include <vector>

#include "cpp_optional.h"
#include "cpp_this.h"  // IWYU pragma: keep
#include "cpp_typed_struct.h"
#include "cpp_views.h"
#include "sqldb_enums.h"
#include "sqldb_value.h"

namespace stonks::sqldb {
/**
 * @brief SQL query.
 */
struct Query : public cpp::TypedStruct<std::string> {};

/**
 * @brief Description of SQL table cell.
 */
struct CellDefinition {
  std::string column_name{};
  DataType data_type{};
};

/**
 * @brief SQL query with expected result definition.
 */
struct SelectQuery {
  Query query{};
  std::vector<CellDefinition> column_types{};
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_TYPES_H_
