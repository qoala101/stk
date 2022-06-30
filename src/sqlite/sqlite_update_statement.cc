#include "sqlite_update_statement.h"

#include <sqlite3.h>

#include <stdexcept>
#include <string>
#include <utility>

#include "not_null.hpp"
#include "sqlite_prepared_statement_facade.h"
#include "sqlite_prepared_statement_handle.h"

namespace stonks::sqlite {
SqliteUpdateStatement::SqliteUpdateStatement(
    SqlitePreparedStatementHandle prepared_statement_handle)
    : prepared_statement_handle_{std::move(prepared_statement_handle)} {}

void SqliteUpdateStatement::Execute(const std::vector<sqldb::Value> &params) {
  auto &sqlite_statement = prepared_statement_handle_.GetSqliteStatement();
  auto prepared_statement_facade =
      SqlitePreparedStatementFacade{cpp::assume_not_null(&sqlite_statement)};
  prepared_statement_facade.Reset();
  prepared_statement_facade.BindParams(params);

  const auto result_code = prepared_statement_facade.Step();

  if (result_code != SQLITE_DONE) {
    throw std::runtime_error{"Unexpected update statement result: {}" +
                             std::to_string(result_code)};
  }
}
}  // namespace stonks::sqlite