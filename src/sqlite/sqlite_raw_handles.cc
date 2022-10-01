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
SqliteDbCloser::SqliteDbCloser(
    cpp::NnSp<di::IFactory<log::ILogger>> logger_factory)
    : logger_factory_{std::move(logger_factory).as_nullable()} {
  Ensures(logger_factory != nullptr);
}

void SqliteDbCloser::operator()(sqlite3* sqlite_db) noexcept {
  Expects(sqlite_db != nullptr);
  Expects(logger_factory_ != nullptr);

  auto logger = cpp::AssumeNn(logger_factory_->create());

  try {
    DbFacade{cpp::AssumeNn(std::move(logger_factory_)),
             cpp::AssumeNn(sqlite_db)}
        .Close();
  } catch (const std::exception& e) {
    logger->LogErrorCondition(e.what());
  }

  Ensures(logger_factory_ == nullptr);
}

SqliteStatementFinalizer::SqliteStatementFinalizer(
    cpp::NnUp<log::ILogger> logger)
    : logger_{std::move(logger).as_nullable()} {
  Ensures(logger_ != nullptr);
}

void SqliteStatementFinalizer::operator()(
    sqlite3_stmt* sqlite_statement) noexcept {
  Expects(sqlite_statement != nullptr);
  Expects(logger_ != nullptr);

  try {
    PreparedStatementFacade{cpp::AssumeNn(sqlite_statement)}.Finalize();
  } catch (const std::exception& e) {
    logger_->LogErrorCondition(e.what());
  }
}
}  // namespace stonks::sqlite::detail