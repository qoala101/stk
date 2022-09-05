#ifndef STONKS_SQLITE_SQLITE_FILE_DB_FACTORY_H_
#define STONKS_SQLITE_SQLITE_FILE_DB_FACTORY_H_

#include <string>

#include "cpp_not_null.h"
#include "sqldb_i_db.h"
#include "sqldb_i_factory.h"
#include "sqldb_i_query_builder.h"

namespace stonks::sqlite {
/**
 * @brief Creates file DBs.
 * @copydoc sqldb::IFactory
 */
// NOLINTNEXTLINE(*-special-member-functions)
class FileDbFactory : public sqldb::IFactory {
 public:
  /**
   * @param db_file_path Path to the SQLite file from which DB would be read.
   */
  explicit FileDbFactory(std::string db_file_path);

  /**
   * @copydoc sqldb::IDb::CreateDb
   */
  [[nodiscard]] auto CreateDb() const -> cpp::NnUp<sqldb::IDb> override;

  /**
   * @copydoc sqldb::IDb::CreateQueryBuilder
   */
  [[nodiscard]] auto CreateQueryBuilder() const
      -> cpp::NnUp<sqldb::IQueryBuilder> override;

 private:
  std::string db_file_path_{};
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_FILE_DB_FACTORY_H_
