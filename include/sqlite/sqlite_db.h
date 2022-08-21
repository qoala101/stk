#ifndef STONKS_SQLITE_SQLITE_DB_H_
#define STONKS_SQLITE_SQLITE_DB_H_

#include <string>
#include <string_view>

#include "ccutils_not_null.h"
#include "sqldb_i_db.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_row_definition.h"
#include "sqlite_types.h"

namespace stonks::sqlite {
/**
 * @copydoc sqldb::IDb
 */
class Db : public sqldb::IDb {
 public:
  /**
   * @brief Creates wrapper for SQLite DB.
   */
  explicit Db(SqliteDbHandle sqlite_db);

  Db(const Db &) = delete;
  Db(Db &&) noexcept = default;

  auto operator=(const Db &) -> Db & = delete;
  auto operator=(Db &&) noexcept -> Db & = default;

  /**
   * @brief Closes SQLite DB.
   * @remark Doesn't write DB to file. It should be done manually.
   */
  ~Db() noexcept override;

  /**
   * @copydoc sqldb::IDb::PrepareStatement
   */
  [[nodiscard]] auto PrepareStatement(std::string query,
                                      sqldb::RowDefinition result_definition)
      -> ccutils::NnUp<sqldb::ISelectStatement> override;

  /**
   * @copydoc sqldb::IDb::PrepareStatement
   */
  [[nodiscard]] auto PrepareStatement(std::string query)
      -> ccutils::NnUp<sqldb::IUpdateStatement> override;

  /**
   * @brief
   *
   */
  void WriteToFile(std::string_view file_path) override;

 private:
  class Impl;
  ccutils::NnUp<Impl> impl_;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_DB_H_
