#include "sqlite_native_handles.h"

#include <exception>
#include <gsl/assert>
#include <memory>
#include <not_null.hpp>
#include <utility>

#include "cpp_not_null.h"
#include "sqlite_native_db_facade.h"
#include "sqlite_native_statement_facade.h"

namespace vh::sqlite::detail {
NativeDbCloser::NativeDbCloser(cpp::NnSp<log::ILogger> logger)
    : logger_{std::move(logger).as_nullable()} {
  Ensures(logger_ != nullptr);
}

void NativeDbCloser::operator()(sqlite3* db) noexcept {
  Expects(db != nullptr);
  Expects(logger_ != nullptr);

  const auto native_db_facade = NativeDbFacade{cpp::AssumeNn(logger_)};

  try {
    native_db_facade.Close(*db);
  } catch (const std::exception& e) {
    logger_->LogErrorCondition(e.what());
  }
}

NativeStatementFinalizer::NativeStatementFinalizer(
    cpp::NnSp<log::ILogger> logger)
    : logger_{std::move(logger).as_nullable()} {
  Ensures(logger_ != nullptr);
}

void NativeStatementFinalizer::operator()(sqlite3_stmt* statement) noexcept {
  Expects(statement != nullptr);
  Expects(logger_ != nullptr);

  try {
    NativeStatementFacade::Finalize(*statement);
  } catch (const std::exception& e) {
    logger_->LogErrorCondition(e.what());
  }
}
}  // namespace vh::sqlite::detail