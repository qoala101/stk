#ifndef STONKS_SQLITE_SQLITE_SELECT_STATEMENT_H_
#define STONKS_SQLITE_SQLITE_SELECT_STATEMENT_H_

#include <vector>

#include "sqldb_i_select_statement.h"
#include "sqldb_rows.h"
#include "sqldb_types.h"
#include "sqldb_value.h"
#include "sqlite_ps_common_impl.h"

namespace stonks::sqlite {
/**
 * @copydoc sqldb::ISelectStatement
 */
class SelectStatement : public sqldb::ISelectStatement {
 public:
  /**
   * @param result_definition Description of rows to be parsed from prepared
   * statement result.
   */
  SelectStatement(ps::CommonImpl impl,
                  const sqldb::ResultDefinition &result_definition);

  /**
   * @copydoc sqldb::ISelectStatement::Execute
   */
  auto Execute [[nodiscard]] (std::vector<sqldb::Value> params) const
      -> sqldb::Rows override;

 private:
  ps::CommonImpl impl_;
  std::vector<sqldb::Column> result_columns_{};
  std::vector<sqldb::DataTypeVariant> result_types_{};
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_SELECT_STATEMENT_H_
