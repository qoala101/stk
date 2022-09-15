#include "sqlite_raw_handles.h"

#include <bits/exception.h>

#include <gsl/assert>
#include <memory>
#include <type_traits>
#include <utility>

#include "cpp_not_null.h"
#include "sqlite_db_facade.h"
#include "sqlite_prepared_statement_facade.h"

namespace stonks::sqlite::detail {
SqliteDbCloser::SqliteDbCloser(cpp::NnSp<log::ILogger> logger)
    : logger_{std::move(logger).as_nullable()} {
  Ensures(logger_ != nullptr);
}

void SqliteDbCloser::operator()(sqlite3* sqlite_db) noexcept {
  Expects(logger_ != nullptr);

  try {
    DbFacade{cpp::AssumeNn(logger_), cpp::AssumeNn(sqlite_db)}.Close();
  } catch (const std::exception& e) {
    logger_->LogErrorCondition(e.what());
  }
}

SqliteStatementFinalizer::SqliteStatementFinalizer(
    cpp::NnSp<log::ILogger> logger)
    : logger_{std::move(logger).as_nullable()} {
  Ensures(logger_ != nullptr);
}

void SqliteStatementFinalizer::operator()(
    sqlite3_stmt* sqlite_statement) noexcept {
  Expects(logger_ != nullptr);

  try {
    PreparedStatementFacade{cpp::AssumeNn(sqlite_statement)}.Finalize();
  } catch (const std::exception& e) {
    logger_->LogErrorCondition(e.what());
  }
}
}  // namespace stonks::sqlite::detail