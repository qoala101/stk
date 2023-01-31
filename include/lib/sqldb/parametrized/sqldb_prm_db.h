#ifndef VH_SQLDB_PRM_DB_H_
#define VH_SQLDB_PRM_DB_H_

#include "cpp_not_null.h"
#include "cpp_this.h"
#include "sqldb_i_db.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_prm_types.h"
#include "sqldb_types.h"

namespace vh::sqldb::prm {
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
  auto PrepareStatement [[nodiscard]] (Parametrized<SelectQuery> query)
  -> cpp::NnUp<ISelectStatement>;

  /**
   * @brief Prepares parametrized update statement.
   */
  auto PrepareStatement [[nodiscard]] (Parametrized<Query> query)
  -> cpp::NnUp<IUpdateStatement>;

  /**
   * @brief Gives wrapped DB.
   */
  auto GetDb [[nodiscard]] () const -> const IDb &;
  auto GetDb [[nodiscard]] () -> IDb &;

 private:
  static auto GetDbImpl [[nodiscard]] (cpp::This<Db> auto &t) -> auto &;

  cpp::NnUp<IDb> db_;
};
}  // namespace vh::sqldb::prm

#endif  // VH_SQLDB_PRM_DB_H_
