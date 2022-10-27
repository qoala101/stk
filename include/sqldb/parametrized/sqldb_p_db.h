#ifndef STONKS_SQLDB_PARAMETRIZED_SQLDB_P_DB_H_
#define STONKS_SQLDB_PARAMETRIZED_SQLDB_P_DB_H_

#include <vector>

#include "cpp_not_null.h"
#include "sqldb_i_db.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_p_types.h"

namespace stonks::sqldb::p {
/**
 * @brief Wrapper over DB which checks parameters of prepared statements.
 */
class Db {
 public:
  /**
   * @param db Wrapped DB.
   */
  explicit Db(cpp::NnUp<IDb> db);

  /**
   * @brief Prepares parametrized select statement.
   */
  [[nodiscard]] auto PrepareStatement(Parametrized<SelectQuery> query)
      -> cpp::NnUp<ISelectStatement>;

  /**
   * @brief Prepares parametrized update statement.
   */
  [[nodiscard]] auto PrepareStatement(Parametrized<Query> query)
      -> cpp::NnUp<IUpdateStatement>;

  /**
   * @brief Gives wrapped DB.
   */
  [[nodiscard]] auto GetDb() const -> const IDb &;
  [[nodiscard]] auto GetDb() -> IDb &;

 private:
  [[nodiscard]] static auto GetDbImpl(cpp::This<Db> auto &t) -> auto &;

  cpp::NnUp<IDb> db_;
};
}  // namespace stonks::sqldb::p

#endif  // STONKS_SQLDB_PARAMETRIZED_SQLDB_P_DB_H_
