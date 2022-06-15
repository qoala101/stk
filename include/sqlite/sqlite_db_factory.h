#ifndef STONKS_SQLITE_SQLITE_DB_FACTORY_H_
#define STONKS_SQLITE_SQLITE_DB_FACTORY_H_

#include <memory>
#include <string_view>

#include "sqldb_db.h"
#include "sqldb_factory.h"
#include "sqldb_query_builder.h"

namespace stonks::sqlite {
/**
 * @brief Abstract factory for DB related entities.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class SqliteDbFactory : public sqldb::DbFactory {
 public:
  /**
   * @copydoc Db::LoadDbFromFile
   */
  [[nodiscard]] auto LoadDbFromFile(std::string_view file_path)
      -> std::unique_ptr<sqldb::Db> override;

  /**
   * @copydoc Db::CreateQueryBuilder
   */
  [[nodiscard]] auto CreateQueryBuilder()
      -> std::unique_ptr<sqldb::QueryBuilder> override;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_DB_FACTORY_H_
