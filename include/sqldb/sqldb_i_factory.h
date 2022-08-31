#ifndef STONKS_SQLDB_SQLDB_I_FACTORY_H_
#define STONKS_SQLDB_SQLDB_I_FACTORY_H_

#include "cpp_not_null.h"
#include "sqldb_i_db.h"
#include "sqldb_i_query_builder.h"

namespace stonks::sqldb {
/**
 * @brief Abstract factory for DB related entities.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IFactory {
 public:
  virtual ~IFactory() = default;

  /**
   * @brief Loads DB from specified file.
   */
  [[nodiscard]] virtual auto LoadDbFromFile(std::string_view file_path) const
      -> cpp::NnUp<IDb> = 0;

  /**
   * @brief Creates query builder which knows how to build queries for DB.
   */
  [[nodiscard]] virtual auto CreateQueryBuilder() const
      -> cpp::NnUp<IQueryBuilder> = 0;
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_I_FACTORY_H_
