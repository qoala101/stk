#include "sqlite_native_handles.h"

#include <exception>
#include <gsl/assert>
#include <memory>
#include <not_null.hpp>
#include <utility>

#include "cpp_not_null.h"
#include "sqlite_native_db_facade.h"
#include "sqlite_native_statement_facade.h"

namespace stonks::sqlite::detail {
NativeDbCloser::NativeDbCloser(di::Factory<log::ILogger> logger_factory)
    : logger_factory_{std::move(logger_factory)} {
  Ensures(logger_factory_.has_value());
}

void NativeDbCloser::operator()(sqlite3* native_db) noexcept {
  Expects(native_db != nullptr);
  Expects(logger_factory_.has_value());

  auto logger = logger_factory_->Create();

  try {
    NativeDbFacade{std::move(*logger_factory_), cpp::AssumeNn(native_db)}.Close();
  } catch (const std::exception& e) {
    logger->LogErrorCondition(e.what());
  }
}

NativeStatementFinalizer::NativeStatementFinalizer(
    cpp::NnUp<log::ILogger> logger)
    : logger_{std::move(logger).as_nullable()} {
  Ensures(logger_ != nullptr);
}

void NativeStatementFinalizer::operator()(
    sqlite3_stmt* native_statement) noexcept {
  Expects(native_statement != nullptr);
  Expects(logger_ != nullptr);

  try {
    NativeStatementFacade{cpp::AssumeNn(native_statement)}.Finalize();
  } catch (const std::exception& e) {
    logger_->LogErrorCondition(e.what());
  }
}
}  // namespace stonks::sqlite::detail