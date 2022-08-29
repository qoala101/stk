#ifndef STONKS_SQLITE_SQLITE_DB_H_
#define STONKS_SQLITE_SQLITE_DB_H_

#include <string>
#include <string_view>

#include "cpp_not_null.h"
#include "sqldb_i_db.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_row_definition.h"
#include "sqlite_db_facade.h"
#include "sqlite_types.h"

class sqlite3;  // IWYU pragma: keep
                // IWYU pragma: no_include <sqlite3.h>

namespace stonks::sqlite {
/**
 * @copydoc sqldb::IDb
 */
class Db : public sqldb::IDb {
 public:
  /**
   * @brief Creates wrapper for SQLite DB.
   */
  explicit Db(SqliteDbSharedHandle sqlite_db_handle);

  Db(const Db &) = delete;
  Db(Db &&) noexcept = default;

  auto operator=(const Db &) -> Db & = delete;
  auto operator=(Db &&) noexcept -> Db & = default;

  /**
   * @brief Closes SQLite DB.
   * @remark Doesn't write DB to file. It should be done manually.
   */
  ~Db() override = default;

  /**
   * @copydoc sqldb::IDb::PrepareStatement
   */
  [[nodiscard]] auto PrepareStatement(std::string query,
                                      sqldb::RowDefinition result_definition)
      -> cpp::NnUp<sqldb::ISelectStatement> override;

  /**
   * @copydoc sqldb::IDb::PrepareStatement
   */
  [[nodiscard]] auto PrepareStatement(std::string query)
      -> cpp::NnUp<sqldb::IUpdateStatement> override;

  /**
   * @copydoc sqldb::IDb::WriteToFile
   */
  void WriteToFile(std::string_view file_path) const override;

 private:
  SqliteDbSharedHandle sqlite_db_handle_;
  DbFacade sqlite_db_facade_;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_DB_H_
