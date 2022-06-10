#ifndef STONKS_DB_SQLITE_SQLITE_DB_FACTORY_H_
#define STONKS_DB_SQLITE_SQLITE_DB_FACTORY_H_

#include <memory>
#include <string_view>

#include "db.h"
#include "db_factory.h"
#include "db_query_builder.h"

namespace stonks::db::sqlite {
/**
 * @brief Abstract factory for DB related entities.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class SqliteDbFactory : public DbFactory {
 public:
  /**
   * @copydoc Db::LoadDbFromFile
   */
  [[nodiscard]] auto LoadDbFromFile(std::string_view file_path)
      -> std::unique_ptr<Db> override;

  /**
   * @copydoc Db::CreateQueryBuilder
   */
  [[nodiscard]] auto CreateQueryBuilder()
      -> std::unique_ptr<QueryBuilder> override;
};
}  // namespace stonks::db::sqlite

#endif  // STONKS_DB_SQLITE_SQLITE_DB_FACTORY_H_
