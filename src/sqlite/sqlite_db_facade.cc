#include "sqlite_db_facade.h"

#include <bits/exception.h>
#include <fmt/format.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <sqlite3.h>

#include <gsl/assert>
#include <gsl/util>
#include <memory>
#include <string>

#include "cpp_message_exception.h"
#include "cpp_not_null.h"
#include "not_null.hpp"
#include "sqlite_db_handles_factory.h"
#include "sqlite_raw_handles.h"

namespace stonks::sqlite {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger = spdlog::stdout_color_mt("sqlite::DbFacade");
  return *logger;
}

[[nodiscard]] auto GetAssociatedFileName(sqlite3 &sqlite_db) -> std::string {
  const auto *const file_name = sqlite3_db_filename(&sqlite_db, nullptr);

  if (file_name == nullptr) {
    return {};
  }

  return file_name;
}

[[nodiscard]] auto GetPrintableFileName(sqlite3 &sqlite_db) -> std::string {
  auto file_name = GetAssociatedFileName(sqlite_db);

  if (file_name.empty()) {
    return "memory";
  }

  return file_name;
}
}  // namespace

DbFacade::DbFacade(cpp::Nn<sqlite3 *> sqlite_db)
    : sqlite_db_{sqlite_db.as_nullable()} {
  Ensures(sqlite_db_ != nullptr);
}

void DbFacade::WriteToFile(std::string_view file_path) const {
  Expects(sqlite_db_ != nullptr);

  auto file_db_handle = db_handles_factory::CreateHandleToFileDb(file_path);
  DbFacade{cpp::AssumeNn(file_db_handle.get())}.CopyDataFrom(*sqlite_db_);

  Logger().info("Stored DB to {}", file_path.data());
}

void DbFacade::CopyDataFrom(sqlite3 &other_db) const {
  Expects(sqlite_db_ != nullptr);

  auto *backup = sqlite3_backup_init(sqlite_db_, "main", &other_db, "main");
  sqlite3_backup_step(backup, -1);
  const auto result_code = sqlite3_backup_finish(backup);

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{"Couldn't copy data from other DB: " +
                                std::to_string(result_code)};
  }
}

auto DbFacade::CreatePreparedStatement(std::string_view query) const
    -> SqliteStatementHandle {
  Expects(sqlite_db_ != nullptr);

  auto *sqlite_statement = (sqlite3_stmt *){};
  sqlite3_prepare_v3(sqlite_db_, query.data(),
                     gsl::narrow_cast<int>(query.length()) + 1,
                     SQLITE_PREPARE_PERSISTENT, &sqlite_statement, nullptr);

  if (sqlite_statement == nullptr) {
    throw cpp::MessageException{
        std::string{"Couldn't prepare the statement for query: "} +
        query.data()};
  }

  Logger().info("Prepared statement for query: {}", query.data());
  return SqliteStatementHandle{cpp::AssumeNn(sqlite_statement)};
}

void DbFacade::EnableForeignKeys() const {
  Expects(sqlite_db_ != nullptr);

  const auto result_code = sqlite3_exec(sqlite_db_, "PRAGMA foreign_keys = ON",
                                        nullptr, nullptr, nullptr);

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{"Couldn't set foreign_keys pragma on new DB: " +
                                std::to_string(result_code)};
  }
}

void DbFacade::Close() {
  Expects(sqlite_db_ != nullptr);

  const auto file_name = GetPrintableFileName(*sqlite_db_);
  const auto result_code = sqlite3_close(sqlite_db_);

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{"Couldn't close DB from " + file_name};
  }

  Logger().info("Closed DB from {}", file_name);

  sqlite_db_ = nullptr;
  Ensures(sqlite_db_ == nullptr);
}
}  // namespace stonks::sqlite