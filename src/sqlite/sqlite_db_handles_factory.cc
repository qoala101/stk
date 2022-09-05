#include "sqlite_db_handles_factory.h"

#include <bits/exception.h>
#include <fmt/format.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <sqlite3.h>

#include <filesystem>
#include <gsl/assert>
#include <memory>
#include <string>

#include "cpp_message_exception.h"
#include "cpp_not_null.h"
#include "not_null.hpp"
#include "sqlite_db_facade.h"
#include "sqlite_raw_handles.h"

namespace stonks::sqlite::db_handles_factory {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger = spdlog::stdout_color_mt("sqlite::ReadFromFile");
  return *logger;
}
}  // namespace

auto CreateInMemoryDb() -> SqliteDbHandle {
  auto *in_memory_db = (sqlite3 *){};
  const auto result_code = sqlite3_open(":memory:", &in_memory_db);

  if ((in_memory_db == nullptr) || (result_code != SQLITE_OK)) {
    throw cpp::MessageException{"Couldn't create in memory DB: " +
                                std::to_string(result_code)};
  }

  DbFacade{cpp::AssumeNn(in_memory_db)}.EnableForeignKeys();

  return SqliteDbHandle{cpp::AssumeNn(in_memory_db)};
}

auto CreateHandleToFileDb(std::string_view file_path) -> SqliteDbHandle {
  Expects(!file_path.empty());

  auto *file_db = (sqlite3 *){};
  const auto result_code = sqlite3_open(file_path.data(), &file_db);

  if ((file_db == nullptr) || (result_code != SQLITE_OK)) {
    throw cpp::MessageException{std::string{"Couldn't read DB from file "} +
                                file_path.data() + ": " +
                                std::to_string(result_code)};
  }

  return SqliteDbHandle{cpp::AssumeNn(file_db)};
}

auto LoadDbFromFileToMemory(std::string_view file_path) -> SqliteDbHandle {
  Expects(!file_path.empty());

  auto in_memory_db_handle = CreateInMemoryDb();

  if (const auto db_is_new = !std::filesystem::exists(file_path)) {
    Logger().info("Created new DB for {}", file_path);
    return in_memory_db_handle;
  }

  auto file_db_handle = CreateHandleToFileDb(file_path.data());
  DbFacade{cpp::AssumeNn(in_memory_db_handle.get())}.CopyDataFrom(
      *file_db_handle);

  Logger().info("Loaded DB from {}", file_path.data());
  return in_memory_db_handle;
}
}  // namespace stonks::sqlite::db_handles_factory