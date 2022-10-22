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
 * @brief Value of single column cell.
 */
struct Cell {
  std::string column_name{};
  Value value{};
};

/**
 * @brief Description of SQL table cell.
 */
struct CellDefinition {
  std::string column_name{};
  DataType data_type{};

 private:
  [[nodiscard]] friend auto operator==(const CellDefinition &,
                                       const CellDefinition &)
      -> bool = default;
};

using RowDefinition = std::vector<CellDefinition>;
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_TYPES_H_
