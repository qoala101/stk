#ifndef STONKS_SQLDB_SQLDB_I_DB_H_
#define STONKS_SQLDB_SQLDB_I_DB_H_

#include <optional>
#include <string>
#include <vector>

#include "ccutils_not_null.h"
#include "not_null.hpp"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_row_definition.h"

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
  [[nodiscard]] virtual auto PrepareStatement(std::string query,
                                              RowDefinition result_definition)
      -> ccutils::NnUp<ISelectStatement> = 0;

  /**
   * @brief Creates prepared statement which can then be called to execute the
   * update query on DB.
   */
  [[nodiscard]] virtual auto PrepareStatement(std::string query)
      -> ccutils::NnUp<IUpdateStatement> = 0;

  /**
   * @brief Stores this DB to the specified file.
   */
  virtual void WriteToFile(std::string_view file_path) = 0;

 protected:
  explicit IDb() = default;
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_I_DB_H_
