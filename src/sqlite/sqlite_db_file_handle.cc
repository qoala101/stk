#include "sqlite_db_file_handle.h"

#include <bits/exception.h>

#include <memory>
#include <utility>

#include "cpp_not_null.h"
#include "not_null.hpp"
#include "sqlite_db_facade.h"

namespace stonks::sqlite {
SqliteDbFileHandle::SqliteDbFileHandle(cpp::NnSp<log::ILogger> logger,
                                       SqliteDbHandle sqlite_db_handle,
                                       FilePath file_path)
    : logger_{std::move(logger)},
      sqlite_db_handle_{std::move(sqlite_db_handle)},
      file_path_{std::move(file_path)} {}

SqliteDbFileHandle::~SqliteDbFileHandle() {
  auto* sqlite_db = sqlite_db_handle_.get();

  if (const auto handle_was_moved = sqlite_db == nullptr) {
    return;
  }

  try {
    DbFacade{logger_, cpp::AssumeNn(sqlite_db)}.WriteToFile(file_path_);
  } catch (const std::exception& e) {
    logger_->LogErrorCondition(e.what());
  }
}

template <cpp::DecaysTo<SqliteDbFileHandle> This>
auto SqliteDbFileHandle::GetSqliteDbImpl(This& t)
    -> cpp::CopyConst<This, sqlite3> & {
  return *t.sqlite_db_handle_;
}

auto SqliteDbFileHandle::GetSqliteDb() const -> const sqlite3& {
  return GetSqliteDbImpl(*this);
}

auto SqliteDbFileHandle::GetSqliteDb() -> sqlite3& {
  return GetSqliteDbImpl(*this);
}

auto SqliteDbFileHandle::GetFilePath() const -> const FilePath& {
  return file_path_;
}
}  // namespace stonks::sqlite