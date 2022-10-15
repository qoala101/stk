#include "sqlite_update_statement.h"

#include <fmt/core.h>
#include <sqlite3.h>

#include <utility>

#include "cpp_message_exception.h"
#include "sqlite_prepared_statement_facade.h"
#include "sqlite_ps_common_impl.h"
#include "sqlite_types.h"

namespace stonks::sqlite {
UpdateStatement::UpdateStatement(ps::CommonImpl impl)
    : impl_{std::move(impl)} {}

void UpdateStatement::Execute(std::vector<sqldb::Value> params) const {
  impl_.BeforeExecution(params);

  const auto &prepared_statement_facade = impl_.GetPreparedStatementFacade();
  const auto result_code = prepared_statement_facade.Step();

  if (result_code != SQLITE_DONE) {
    throw cpp::MessageException{
        fmt::format("Unexpected update statement result: {}", result_code)};
  }
}
}  // namespace stonks::sqlite