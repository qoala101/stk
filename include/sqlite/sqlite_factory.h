#ifndef STONKS_SQLITE_SQLITE_DB_FACTORY_H_
#define STONKS_SQLITE_SQLITE_DB_FACTORY_H_

#include <string_view>

#include "cpp_not_null.h"
#include "sqldb_i_db.h"
#include "sqldb_i_factory.h"
#include "sqldb_i_query_builder.h"

namespace stonks::sqlite {
/**
 * @brief sqldb::IFactory
 */
// NOLINTNEXTLINE(*-special-member-functions)
class Factory : public sqldb::IFactory {
 public:
  /**
   * @copydoc sqldb::IDb::LoadDbFromFile
   */
  [[nodiscard]] auto LoadDbFromFile(std::string_view file_path) const
      -> cpp::NnUp<sqldb::IDb> override;

  /**
   * @copydoc sqldb::IDb::CreateQueryBuilder
   */
  [[nodiscard]] auto CreateQueryBuilder() const
      -> cpp::NnUp<sqldb::IQueryBuilder> override;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_DB_FACTORY_H_
