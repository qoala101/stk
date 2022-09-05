#include "sqlite_db_file_handle.h"

#include <bits/exception.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>
#include <utility>

#include "cpp_not_null.h"
#include "sqlite_db_facade.h"

namespace stonks::sqlite {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger& {
  static auto logger = spdlog::stdout_color_mt("sqlite::RawHandles");
  return *logger;
}
}  // namespace

SqliteDbFileHandle::SqliteDbFileHandle(SqliteDbHandle sqlite_db_handle,
                                       std::string file_path)
    : sqlite_db_handle_{std::move(sqlite_db_handle)},
      file_path_{std::move(file_path)} {}

SqliteDbFileHandle::~SqliteDbFileHandle() {
  auto* sqlite_db = sqlite_db_handle_.get();

  if (const auto handle_was_moved = sqlite_db == nullptr) {
    return;
  }

  try {
    DbFacade{cpp::AssumeNn(sqlite_db)}.WriteToFile(file_path_);
  } catch (const std::exception& e) {
    Logger().error(e.what());
  }
}

auto SqliteDbFileHandle::GetSqliteDbImpl(
    cpp::DecaysTo<SqliteDbFileHandle> auto&& t)
    -> cpp::CopyConst<decltype(t), sqlite3&> {
  return *std::forward<decltype(t)>(t).sqlite_db_handle_;
}

auto SqliteDbFileHandle::GetSqliteDb() const -> const sqlite3& {
  return GetSqliteDbImpl(*this);
}

auto SqliteDbFileHandle::GetSqliteDb() -> sqlite3& {
  return GetSqliteDbImpl(*this);
}

auto SqliteDbFileHandle::GetFilePath() const -> const std::string& {
  return file_path_;
}
}  // namespace stonks::sqlite