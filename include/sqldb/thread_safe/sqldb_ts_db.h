#ifndef STONKS_SQLDB_THREAD_SAFE_SQLDB_TS_DB_H_
#define STONKS_SQLDB_THREAD_SAFE_SQLDB_TS_DB_H_

#include "cpp_not_null.h"
#include "sqldb_i_db.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_types.h"

namespace stonks::sqldb::ts {
/**
 * @brief DB wrapper which adds synchronization layer.
 */
class Db : public IDb {
 public:
  /**
   * @param db DB to wrap.
   */
  explicit Db(cpp::NnUp<IDb> db);

  /**
   * @copydoc IDb::PrepareStatement
   */
  auto PrepareStatement [[nodiscard]] (SelectQuery query)
  -> cpp::NnUp<ISelectStatement> override;

  /**
   * @copydoc IDb::PrepareStatement
   */
  auto PrepareStatement [[nodiscard]] (Query query)
  -> cpp::NnUp<IUpdateStatement> override;

 private:
  cpp::NnUp<IDb> db_;
};
}  // namespace stonks::sqldb::ts

#endif  // STONKS_SQLDB_THREAD_SAFE_SQLDB_TS_DB_H_
