#ifndef STONKS_SQLDB_SQLDB_I_DB_H_
#define STONKS_SQLDB_SQLDB_I_DB_H_

#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include "not_null.hpp"
#include "sqldb_i_query_builder.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_row_definition.h"
#include "sqldb_types.h"

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
  [[nodiscard]] virtual auto PrepareStatement(
      std::string_view query, const RowDefinition &result_definition)
      -> cpp::not_null<std::unique_ptr<ISelectStatement>> = 0;

  /**
   * @brief Creates prepared statement which can then be called to execute the
   * update query on DB.
   */
  [[nodiscard]] virtual auto PrepareStatement(std::string_view query)
      -> cpp::not_null<std::unique_ptr<IUpdateStatement>> = 0;

  /**
   * @brief Stores this DB to the specified file.
   */
  virtual void WriteToFile(std::string_view file_path) = 0;
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_I_DB_H_
