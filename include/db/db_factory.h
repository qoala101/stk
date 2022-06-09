#ifndef STONKS_DB_SQLITE_DB_FACTORY_H_
#define STONKS_DB_SQLITE_DB_FACTORY_H_

#include <memory>

#include "db.h"
#include "db_query_builder.h"

namespace stonks::db {
/**
 * @brief Abstract factory for DB related entities.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class DbFactory {
 public:
  virtual ~DbFactory() = default;

  /**
   * @brief Loads DB from specified file.
   */
  [[nodiscard]] virtual auto LoadDbFromFile(std::string_view file_path)
      -> std::unique_ptr<Db> = 0;

  /**
   * @brief Creates query builder which knows how to build queries for DB.
   */
  [[nodiscard]] virtual auto CreateQueryBuilder()
      -> std::unique_ptr<QueryBuilder> = 0;
};
}  // namespace stonks::db

#endif  // STONKS_DB_SQLITE_DB_FACTORY_H_
