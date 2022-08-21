#include "sqlite_read_from_file.h"

#include <bits/exception.h>
#include <fmt/format.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <sqlite3.h>

#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>

#include "ccutils_not_null.h"
#include "ccutils_smart_pointers.h"
#include "not_null.hpp"
#include "sqlite_db_facade.h"

namespace stonks::sqlite::read_from_file {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger = spdlog::stdout_color_mt("sqlite::Utils");
  return *logger;
}

[[nodiscard]] auto OpenSqliteInMemoryDb() -> SqliteDbHandle {
  auto *in_memory_db = (sqlite3 *){};
  sqlite3_open(":memory:", &in_memory_db);

  if (in_memory_db == nullptr) {
    throw std::runtime_error{"Couldn't create in memory DB"};
  }

  return ccutils::AssumeNn(ccutils::Up<sqlite3, SqliteDbCloser>{in_memory_db});
}

void UpdateNewDb(sqlite3 &sqlite_db) {
  const auto result_code = sqlite3_exec(&sqlite_db, "PRAGMA foreign_keys = ON",
                                        nullptr, nullptr, nullptr);

  if (result_code != SQLITE_OK) {
    throw std::runtime_error{"Couldn't set foreign_keys pragma on new DB"};
  }
}
}  // namespace

auto OpenSqliteDbFromFile(std::string_view file_path) -> SqliteDbHandle {
  auto *file_db = (sqlite3 *){};
  sqlite3_open(file_path.data(), &file_db);

  if (file_db == nullptr) {
    throw std::runtime_error{std::string{"Couldn't read DB from file "} +
                             file_path.data() + ": " + sqlite3_errmsg(file_db)};
  }

  return ccutils::AssumeNn(ccutils::Up<sqlite3, SqliteDbCloser>{file_db});
}

auto LoadSqliteDbFromFileToMemory(std::string_view file_path)
    -> SqliteDbHandle {
  auto in_memory_db = OpenSqliteInMemoryDb();

  if (const auto db_is_new = !std::filesystem::exists(file_path)) {
    UpdateNewDb(*in_memory_db);
    Logger().info("Created new DB for {}", file_path.data());
    return in_memory_db;
  }

  auto file_db = OpenSqliteDbFromFile(file_path);
  DbFacade{ccutils::AssumeNn(in_memory_db.get())}.CopyDataFrom(*file_db);

  Logger().info("Loaded DB from {}", file_path.data());
  return in_memory_db;
}
}  // namespace stonks::sqlite::read_from_file