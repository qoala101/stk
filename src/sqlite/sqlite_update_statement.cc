#include "sqlite_update_statement.h"

#include <sqlite3.h>

#include <stdexcept>
#include <string>
#include <utility>

#include "ccutils_not_null.h"
#include "sqlite_prepared_statement_facade.h"
#include "sqlite_prepared_statement_handle.h"

namespace stonks::sqlite {
UpdateStatement::UpdateStatement(
    PreparedStatementHandle prepared_statement_handle)
    : prepared_statement_handle_{std::move(prepared_statement_handle)} {}

void UpdateStatement::Execute(std::vector<sqldb::Value> params) const {
  auto &sqlite_statement = prepared_statement_handle_.GetSqliteStatement();
  auto prepared_statement_facade =
      PreparedStatementFacade{ccutils::AssumeNn(&sqlite_statement)};
  prepared_statement_facade.Reset();
  prepared_statement_facade.BindParams(params);

  const auto result_code = prepared_statement_facade.Step();

  if (result_code != SQLITE_DONE) {
    throw std::runtime_error{"Unexpected update statement result: " +
                             std::to_string(result_code)};
  }
}
}  // namespace stonks::sqlite