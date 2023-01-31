#ifndef STONKS_SQLDB_SQLDB_I_DB_H_
#define STONKS_SQLDB_SQLDB_I_DB_H_

#include "cpp_not_null.h"
#include "sqldb_concepts.h"  // IWYU pragma: keep
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_qb_create.h"
#include "sqldb_query_builder.h"
#include "sqldb_types.h"

namespace vh::sqldb {
/**
 * @brief Generic DB interface.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IDb {
 public:
  virtual ~IDb() = default;

  /**
   * @brief Creates prepared statement which can then be called to execute the
   * select query on DB.
   */
  virtual auto PrepareStatement [[nodiscard]] (SelectQuery query)
  -> cpp::NnUp<ISelectStatement> = 0;

  /**
   * @brief Creates prepared statement which can then be called to execute the
   * update query on DB.
   */
  virtual auto PrepareStatement [[nodiscard]] (Query query)
  -> cpp::NnUp<IUpdateStatement> = 0;

  /**
   * @remark Convenience API for common action.
   */
  template <TableDefinition Table>
  void CreateTableIfNotExists() {
    CreateTableIfNotExistsImpl(query_builder::CreateTable<Table>());
  }

 private:
  void CreateTableIfNotExistsImpl(qb::Create query_builder);
};
}  // namespace vh::sqldb

#endif  // STONKS_SQLDB_SQLDB_I_DB_H_
