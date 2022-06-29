#include "sqlite_db_facade.h"

#include <bits/exception.h>
#include <fmt/format.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <gsl/assert>
#include <gsl/pointers>
#include <memory>
#include <stdexcept>

#include "sqlite_utils.h"

namespace stonks::sqlite {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger = spdlog::stdout_color_mt("SqliteDbFacade");
  return *logger;
}
}  // namespace

SqliteDbFacade::SqliteDbFacade(gsl::strict_not_null<sqlite3 *> sqlite_db)
    : sqlite_db_{sqlite_db} {
  Ensures(sqlite_db_ != nullptr);
}

auto SqliteDbFacade::GetFileName() const -> std::string {
  Expects(sqlite_db_ != nullptr);

  const auto *const file_name = sqlite3_db_filename(sqlite_db_, nullptr);

  if (file_name == nullptr) {
    return {};
  }

  return file_name;
}

void SqliteDbFacade::WriteToFile(std::string_view file_path) const {
  Expects(sqlite_db_ != nullptr);

  auto file_db = utils::OpenSqliteDbFromFile(file_path);
  Expects(file_db != nullptr);

  SqliteDbFacade(gsl::make_strict_not_null(file_db.get()))
      .CopyDataFrom(*sqlite_db_);
  Logger().info("Stored DB to {}", file_path.data());
}

void SqliteDbFacade::CopyDataFrom(sqlite3 &other_db) {
  Expects(sqlite_db_ != nullptr);

  auto *backup = sqlite3_backup_init(sqlite_db_, "main", &other_db, "main");
  sqlite3_backup_step(backup, -1);
  sqlite3_backup_finish(backup);
}

auto SqliteDbFacade::CreatePreparedStatement(std::string_view query)
    -> SqliteStatementHandle {
  Expects(sqlite_db_ != nullptr);

  auto *sqlite_statement = (sqlite3_stmt *){};
  sqlite3_prepare_v3(sqlite_db_, query.data(),
                     static_cast<int>(query.length()) + 1,
                     SQLITE_PREPARE_PERSISTENT, &sqlite_statement, nullptr);

  if (sqlite_statement == nullptr) {
    throw std::runtime_error{
        std::string{"Couldn't prepare the statement for query: "} +
        query.data()};
  }

  auto statement_handle = SqliteStatementHandle{sqlite_statement};
  Ensures(statement_handle != nullptr);
  return statement_handle;
}

void SqliteDbFacade::Close() {
  Expects(sqlite_db_ != nullptr);

  const auto file_name = GetFileName();
  const auto result_code = sqlite3_close(sqlite_db_);

  if (result_code != SQLITE_OK) {
    throw std::runtime_error{"Couldn't close DB from " + file_name};
  }

  Logger().info("Closed DB from {}", file_name);

  sqlite_db_ = nullptr;
  Ensures(sqlite_db_ == nullptr);
}
}  // namespace stonks::sqlite