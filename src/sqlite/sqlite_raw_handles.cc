#include "sqlite_raw_handles.h"

#include <exception>
#include <gsl/assert>
#include <memory>
#include <not_null.hpp>
#include <utility>

#include "cpp_not_null.h"
#include "sqlite_db_facade.h"
#include "sqlite_prepared_statement_facade.h"

namespace stonks::sqlite::detail {
SqliteDbCloser::SqliteDbCloser(di::Factory<log::ILogger> logger_factory)
    : logger_factory_{std::move(logger_factory)} {
  Ensures(logger_factory_.has_value());
}

void SqliteDbCloser::operator()(sqlite3* sqlite_db) noexcept {
  Expects(sqlite_db != nullptr);
  Expects(logger_factory_.has_value());

  auto logger = logger_factory_->Create();

  try {
    DbFacade{std::move(*logger_factory_), cpp::AssumeNn(sqlite_db)}.Close();
  } catch (const std::exception& e) {
    logger->LogErrorCondition(e.what());
  }
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