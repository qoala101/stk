#include "sqlite_update_statement.h"

#include <sqlite3.h>

#include <utility>

#include <fmt/core.h>
#include "cpp_message_exception.h"
#include "cpp_not_null.h"
#include "sqlite_prepared_statement_facade.h"
#include "sqlite_prepared_statement_handle.h"
#include "sqlite_types.h"

namespace stonks::sqlite {
UpdateStatement::UpdateStatement(
    PreparedStatementHandle prepared_statement_handle)
    : prepared_statement_handle_{std::move(prepared_statement_handle)} {}

void UpdateStatement::Execute(std::vector<sqldb::Value> params) const {
  auto &sqlite_statement = prepared_statement_handle_.GetSqliteStatement();
  auto prepared_statement_facade =
      PreparedStatementFacade{cpp::AssumeNn(&sqlite_statement)};
  prepared_statement_facade.Reset();
  prepared_statement_facade.BindParams(params);

  const auto result_code = prepared_statement_facade.Step();

  if (result_code != SQLITE_DONE) {
    throw cpp::MessageException{
        fmt::format("Unexpected update statement result: {}", result_code)};
  }
}
}  // namespace stonks::sqlite