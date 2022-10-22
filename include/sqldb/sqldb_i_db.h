#ifndef STONKS_SQLDB_SQLDB_I_DB_H_
#define STONKS_SQLDB_SQLDB_I_DB_H_

#include <not_null.hpp>
#include <string>
#include <vector>

#include "cpp_not_null.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"

namespace stonks::sqldb {
/**
 * @brief Generic DB interface.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IDb {
 public:
  virtual ~IDb() = default;

  /**
   * @brief Creates prepared statement which can then be called to execute the
   * select query on DB.
   */
  [[nodiscard]] virtual auto PrepareStatement(Query query,
                                              RowDefinition result_definition)
      -> cpp::NnUp<ISelectStatement> = 0;

  /**
   * @brief Creates prepared statement which can then be called to execute the
   * update query on DB.
   */
  [[nodiscard]] virtual auto PrepareStatement(Query query)
      -> cpp::NnUp<IUpdateStatement> = 0;
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_I_DB_H_
