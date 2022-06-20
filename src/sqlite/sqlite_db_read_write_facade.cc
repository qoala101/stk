#include "sqlite_db_read_write_facade.h"

#include <bits/exception.h>
#include <fmt/format.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <filesystem>
#include <gsl/assert>
#include <gsl/pointers>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "sqldb_value.h"

namespace stonks::sqlite {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger = spdlog::stdout_color_mt("Sqlite");
  return *logger;
}

[[nodiscard]] auto GetDbFileName(sqlite3 &sqlite_db) -> std::string {
  const auto *const file_name = sqlite3_db_filename(&sqlite_db, nullptr);

  if (file_name == nullptr) {
    return {};
  }

  return file_name;
}

void CloseDb(gsl::not_null<sqlite3 *> sqlite_db) {
  const auto file_name = GetDbFileName(*sqlite_db);
  const auto result_code = sqlite3_close(sqlite_db);

  if (result_code != SQLITE_OK) {
    throw std::runtime_error{"Couldn't close DB from " + file_name};
  }

  Logger().info("Closed DB from {}", file_name);
}

auto OpenSqliteInMemoryDb() -> SqliteDbHandle {
  auto *in_memory_db = (sqlite3 *){};
  sqlite3_open(":memory:", &in_memory_db);

  if (in_memory_db == nullptr) {
    throw std::runtime_error{"Couldn't create in memory DB"};
  }

  auto handle = SqliteDbHandle{in_memory_db, &CloseDb};
  
  Ensures(handle != nullptr);
  return handle;
}

auto OpenSqliteDbFromFile(std::string_view file_path) -> SqliteDbHandle {
  auto *file_db = (sqlite3 *){};
  sqlite3_open(file_path.data(), &file_db);

  if (file_db == nullptr) {
    throw std::runtime_error{std::string{"Couldn't read DB from file "} +
                             file_path.data() + ": " + sqlite3_errmsg(file_db)};
  }

  auto handle = SqliteDbHandle{file_db, &CloseDb};

  Ensures(handle != nullptr);
  return handle;
}

struct CopyDataArgs {
  sqlite3 &from;
  sqlite3 &to;
};

void CopyData(const CopyDataArgs &args) {
  auto *backup = sqlite3_backup_init(&args.to, "main", &args.from, "main");
  sqlite3_backup_step(backup, -1);
  sqlite3_backup_finish(backup);
}

void UpdateNewDb(sqlite3 &sqlite_db) {
  const auto result_code = sqlite3_exec(&sqlite_db, "PRAGMA foreign_keys = ON",
                                        nullptr, nullptr, nullptr);

  if (result_code != SQLITE_OK) {
    throw std::runtime_error{"Couldn't set foreign_keys pragma on new DB"};
  }
}
}  // namespace

auto SqliteDbReadWriteFacade::ReadSqliteDbFromFile(std::string_view file_path)
    -> SqliteDbHandle {
  auto in_memory_db = OpenSqliteInMemoryDb();

  if (const auto db_is_new = !std::filesystem::exists(file_path)) {
    UpdateNewDb(*in_memory_db);
    Logger().info("Created new DB for {}", file_path.data());

    Ensures(in_memory_db != nullptr);
    return in_memory_db;
  }

  auto file_db = OpenSqliteDbFromFile(file_path);
  CopyData({.from = *file_db, .to = *in_memory_db});
  Logger().info("Loaded DB from {}", file_path.data());

  Ensures(in_memory_db != nullptr);
  return in_memory_db;
}

void SqliteDbReadWriteFacade::WriteSqliteDbToFile(sqlite3 &sqlite_db,
                                                  std::string_view file_path) {
  auto file_db = OpenSqliteDbFromFile(file_path);
  CopyData({.from = sqlite_db, .to = *file_db});
  Logger().info("Stored DB to {}", file_path.data());
}
}  // namespace stonks::sqlite