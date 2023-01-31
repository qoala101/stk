#include "sqlite_prepared_statement_impl.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <memory>
#include <not_null.hpp>
#include <string>
#include <utility>

#include "cpp_not_null.h"
#include "cpp_typed_struct.h"
#include "sqldb_types.h"
#include "sqlite_native_handles.h"
#include "sqlite_native_statement_facade.h"

namespace vh::sqlite {
PreparedStatementImpl::PreparedStatementImpl(
    cpp::NnSp<NativeDbHandleVariant> native_db_handle,
    NativeStatementHandle native_statement_handle, sqldb::Query query,
    cpp::NnSp<log::ILogger> logger, cpp::MutexVariant native_statement_mutex)
    : native_db_handle_{std::move(native_db_handle)},
      native_statement_handle_{
          std::move(native_statement_handle).as_nullable()},
      query_{std::move(query)},
      logger_{std::move(logger)},
      native_statement_mutex_{std::move(native_statement_mutex)} {
  Ensures(native_statement_handle_ != nullptr);
}

PreparedStatementImpl::~PreparedStatementImpl() {
  if (const auto object_was_moved = native_statement_handle_ == nullptr) {
    return;
  }

  const auto lock = native_statement_mutex_.Lock();
  native_statement_handle_.reset();
}

auto PreparedStatementImpl::GetNativeStatement() const -> sqlite3_stmt & {
  return *native_statement_handle_;
}

auto PreparedStatementImpl::PrepareExecutionAndLock(
    const std::vector<sqldb::Value> &params) const -> cpp::LockVariant {
  logger_->LogImportantEvent(fmt::format("Executing query: {}", *query_));

  auto lock = native_statement_mutex_.Lock();
  NativeStatementFacade::Reset(*native_statement_handle_);
  NativeStatementFacade::BindParams(*native_statement_handle_, params);
  return lock;
}
}  // namespace vh::sqlite