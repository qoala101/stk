#include "sqlite_utils.h"

#include <bits/exception.h>
#include <fmt/format.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>

namespace stonks::sqlite::utils {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger = spdlog::stdout_color_mt("SqliteUtils");
  return *logger;
}

const auto kCloseSqliteDb = [](sqlite3 *sqlite_db) {
  sqlite3_close(sqlite_db);
};

auto OpenSqliteDbFromFile(std::string_view file_path)
    -> std::unique_ptr<sqlite3, decltype(kCloseSqliteDb)> {
  auto *file_db = (sqlite3 *){};
  sqlite3_open(file_path.data(), &file_db);

  if (file_db == nullptr) {
    throw std::runtime_error{std::string{"Cannot read DB from file "} +
                             file_path.data() + ": " + sqlite3_errmsg(file_db)};
  }

  return std::unique_ptr<sqlite3, decltype(kCloseSqliteDb)>{file_db};
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
}  // namespace

auto ReadSqliteDbFromFile(std::string_view file_path)
    -> gsl::not_null<sqlite3 *> {
  auto *in_memory_db = (sqlite3 *){};
  sqlite3_open(":memory:", &in_memory_db);

  if (in_memory_db == nullptr) {
    throw std::runtime_error{"Cannot create in memory DB"};
  }

  if (const auto db_is_new = !std::filesystem::exists(file_path)) {
    sqlite3_exec(in_memory_db, "PRAGMA foreign_keys = ON", nullptr, nullptr,
                 nullptr);
    Logger().info("Created new DB for {}", file_path.data());
    return in_memory_db;
  }

  auto file_db = OpenSqliteDbFromFile(file_path);

  CopyData({.from = *file_db, .to = *in_memory_db});
  Logger().info("Loaded DB from {}", file_path.data());
  return in_memory_db;
}

void WriteSqliteDbToFile(sqlite3 &sqlite_db, std::string_view file_path) {
  auto file_db = OpenSqliteDbFromFile(file_path);

  CopyData({.from = sqlite_db, .to = *file_db});
  Logger().info("Stored DB to {}", file_path.data());
}
}  // namespace stonks::sqlite::utils