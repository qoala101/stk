#include "sqlite_types.h"

#include <bits/exception.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <gsl/pointers>

#include "sqlite_db_facade.h"
#include "sqlite_prepared_statement_facade.h"

namespace stonks::sqlite {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger& {
  static auto logger = spdlog::stdout_color_mt("SqliteTypes");
  return *logger;
}
}  // namespace

void SqliteDbCloser::operator()(gsl::not_null<sqlite3*> sqlite_db) noexcept
    try {
  SqliteDbFacade{gsl::make_strict_not_null(sqlite_db)}.Close();
} catch (const std::exception& exception) {
  Logger().error(exception.what());
}

void SqliteStatementFinalizer::operator()(
    gsl::not_null<sqlite3_stmt*> sqlite_statement) noexcept {
  SqlitePreparedStatementFacade{gsl::make_strict_not_null(sqlite_statement)}
      .Finalize();
}
}  // namespace stonks::sqlite