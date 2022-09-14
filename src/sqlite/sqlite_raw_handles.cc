#include "sqlite_raw_handles.h"

#include <bits/exception.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>

#include "sqlite_db_facade.h"
#include "sqlite_prepared_statement_facade.h"

namespace stonks::sqlite {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger& {
  static auto logger = spdlog::stdout_color_mt("sqlite::RawHandles");
  return *logger;
}
}  // namespace

namespace detail {
void SqliteDbCloser::operator()(sqlite3* sqlite_db) noexcept try {
  DbFacade{cpp::CheckNn(sqlite_db)}.Close();
} catch (const std::exception& e) {
  Logger().error(e.what());
}

void SqliteStatementFinalizer::operator()(
    sqlite3_stmt* sqlite_statement) noexcept try {
  PreparedStatementFacade{cpp::CheckNn(sqlite_statement)}.Finalize();
} catch (const std::exception& e) {
  Logger().error(e.what());
}
}  // namespace detail
}  // namespace stonks::sqlite