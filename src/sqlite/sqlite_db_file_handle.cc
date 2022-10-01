#include "sqlite_db_file_handle.h"

#include <bits/exception.h>

#include <gsl/assert>
#include <memory>
#include <utility>

#include "cpp_not_null.h"
#include "di_factory.h"
#include "not_null.hpp"
#include "sqlite_db_facade.h"

namespace stonks::sqlite {
SqliteDbFileHandle::SqliteDbFileHandle(di::Factory<log::ILogger> logger_factory,
                                       SqliteDbHandle sqlite_db_handle,
                                       FilePath file_path)
    : logger_factory_{std::move(logger_factory)},
      sqlite_db_handle_{std::move(sqlite_db_handle)},
      file_path_{std::move(file_path)} {}

SqliteDbFileHandle::~SqliteDbFileHandle() {
  auto* sqlite_db = sqlite_db_handle_.get();

  if (const auto object_was_moved = sqlite_db == nullptr) {
    return;
  }

  auto logger = logger_factory_.Create();

  try {
    DbFacade{std::move(logger_factory_), cpp::AssumeNn(sqlite_db)}.WriteToFile(
        file_path_);
  } catch (const std::exception& e) {
    logger->LogErrorCondition(e.what());
  }
}

template <cpp::DecaysTo<SqliteDbFileHandle> This>
auto SqliteDbFileHandle::GetSqliteDbImpl(This& t)
    -> cpp::CopyConst<This, sqlite3>& {
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