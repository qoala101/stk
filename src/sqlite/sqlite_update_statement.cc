#include "sqlite_update_statement.h"

#include <fmt/core.h>
#include <sqlite3.h>

#include <utility>

#include "cpp_message_exception.h"
#include "sqlite_native_statement_facade.h"
#include "sqlite_prepared_statement_impl.h"
#include "sqlite_types.h"

namespace stonks::sqlite {
UpdateStatement::UpdateStatement(PreparedStatementImpl impl)
    : impl_{std::move(impl)} {}

void UpdateStatement::Execute(std::vector<sqldb::Value> params) const {
  const auto lock = impl_.PrepareExecutionAndLock(params);
  const auto result_code =
      NativeStatementFacade::Step(impl_.GetNativeStatement());

  if (result_code != SQLITE_DONE) {
    throw cpp::MessageException{
        fmt::format("Unexpected update statement result: {}", result_code)};
  }
}
}  // namespace stonks::sqlite