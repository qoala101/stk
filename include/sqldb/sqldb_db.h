#ifndef STONKS_SQLDB_SQLDB_H_
#define STONKS_SQLDB_SQLDB_H_

#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include "sqldb_prepared_statement.h"
#include "sqldb_query_builder.h"
#include "sqldb_types.h"

namespace stonks::sqldb {
/**
 * @brief Generic DB interface.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class Db {
 public:
  virtual ~Db() = default;

  /**
   * @brief Creates prepared statement which can then be called to execute the
   * query on DB.
   */
  [[nodiscard]] virtual auto PrepareStatement(std::string_view query)
      -> std::unique_ptr<PreparedStatement> = 0;

  /**
   * @brief Stores this DB to the specified file.
   */
  virtual void WriteToFile(std::string_view file_path) = 0;
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_H_
