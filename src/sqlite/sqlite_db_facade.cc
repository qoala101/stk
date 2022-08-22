#include "sqlite_db_facade.h"

#include <bits/exception.h>
#include <fmt/format.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <gsl/assert>
#include <gsl/narrow>
#include <memory>
#include <stdexcept>

#include "ccutils_not_null.h"
#include "ccutils_smart_pointers.h"
#include "not_null.hpp"
#include "sqlite_read_from_file.h"

namespace stonks::sqlite {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger = spdlog::stdout_color_mt("sqlite::DbFacade");
  return *logger;
}
}  // namespace

DbFacade::DbFacade(ccutils::Nn<sqlite3 *> sqlite_db)
    : sqlite_db_{sqlite_db.as_nullable()} {
  Ensures(sqlite_db_ != nullptr);
}

auto DbFacade::GetFileName() const -> std::string {
  Expects(sqlite_db_ != nullptr);

  const auto *const file_name = sqlite3_db_filename(sqlite_db_, nullptr);

  if (file_name == nullptr) {
    return {};
  }

  return file_name;
}

void DbFacade::WriteToFile(std::string_view file_path) const {
  Expects(sqlite_db_ != nullptr);

  auto file_db = read_from_file::OpenSqliteDbFromFile(file_path);
  DbFacade(ccutils::AssumeNn(file_db.get())).CopyDataFrom(*sqlite_db_);

  Logger().info("Stored DB to {}", file_path.data());
}

void DbFacade::CopyDataFrom(sqlite3 &other_db) {
  Expects(sqlite_db_ != nullptr);

  auto *backup = sqlite3_backup_init(sqlite_db_, "main", &other_db, "main");
  sqlite3_backup_step(backup, -1);
  sqlite3_backup_finish(backup);
}

auto DbFacade::CreatePreparedStatement(std::string_view query)
    -> SqliteStatementHandle {
  Expects(sqlite_db_ != nullptr);

  auto *sqlite_statement = (sqlite3_stmt *){};
  sqlite3_prepare_v3(sqlite_db_, query.data(),
                     gsl::narrow<int>(query.length()) + 1,
                     SQLITE_PREPARE_PERSISTENT, &sqlite_statement, nullptr);

  if (sqlite_statement == nullptr) {
    throw std::runtime_error{
        std::string{"Couldn't prepare the statement for query: "} +
        query.data()};
  }

  Logger().info("Prepared statement for query: {}", query.data());
  return ccutils::AssumeNn(
      ccutils::Up<sqlite3_stmt, SqliteStatementFinalizer>{sqlite_statement});
}

void DbFacade::Close() {
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