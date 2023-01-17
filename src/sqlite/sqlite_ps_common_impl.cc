#include "sqlite_ps_common_impl.h"

#include <fmt/core.h>

#include <string>
#include <utility>

#include "cpp_not_null.h"
#include "cpp_typed_struct.h"
#include "sqldb_types.h"
#include "sqlite_native_handles.h"
#include "sqlite_native_statement_facade.h"

namespace stonks::sqlite::ps {
CommonImpl::CommonImpl(cpp::NnSp<NativeDbHandleVariant> native_db_handle,
                       NativeStatementHandle native_statement_handle,
                       sqldb::Query query, cpp::NnUp<log::ILogger> logger)
    : native_db_handle_{std::move(native_db_handle)},
      native_statement_handle_{std::move(native_statement_handle)},
      query_{std::move(query)},
      logger_{std::move(logger)} {}

auto CommonImpl::GetNativeStatement() const -> sqlite3_stmt& {
  return *native_statement_handle_;
}

void CommonImpl::BeforeExecution(
    const std::vector<sqldb::Value>& params) const {
  logger_->LogImportantEvent(fmt::format("Executing query: {}", *query_));

  NativeStatementFacade::Reset(*native_statement_handle_);
  NativeStatementFacade::BindParams(*native_statement_handle_, params);
}
}  // namespace stonks::sqlite::ps