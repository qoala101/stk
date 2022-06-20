#ifndef STONKS_SQLDB_SQLDB_DB_FACTORY_H_
#define STONKS_SQLDB_SQLDB_DB_FACTORY_H_

#include <memory>

#include "sqldb_db.h"
#include "sqldb_query_builder.h"

namespace stonks::sqldb {
/**
 * @brief Abstract factory for DB related entities.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IDbFactory {
 public:
  virtual ~IDbFactory() = default;

  /**
   * @brief Loads DB from specified file.
   */
  [[nodiscard]] virtual auto LoadDbFromFile(std::string_view file_path)
      -> std::unique_ptr<IDb> = 0;

  /**
   * @brief Creates query builder which knows how to build queries for DB.
   */
  [[nodiscard]] virtual auto CreateQueryBuilder()
      -> std::unique_ptr<IQueryBuilder> = 0;
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_DB_FACTORY_H_
