#include "sqlite_db_handles_factory.h"

#include <sqlite3.h>

#include <filesystem>
#include <gsl/assert>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

#include "cpp_message_exception.h"
#include "cpp_not_null.h"
#include "cpp_format.h"
#include "not_null.hpp"
#include "sqlite_db_facade.h"
#include "sqlite_raw_handles.h"

namespace stonks::sqlite {
namespace {
using NullableSqliteDbHandle =
    std::decay_t<decltype(std::declval<SqliteDbHandle>().as_nullable())>;
}  // namespace

DbHandlesFactory::DbHandlesFactory(cpp::NnSp<log::ILogger> logger)
    : logger_{std::move(logger)} {}

auto DbHandlesFactory::CreateInMemoryDb() const -> SqliteDbHandle {
  auto *in_memory_db = (sqlite3 *){};
  const auto result_code = sqlite3_open(":memory:", &in_memory_db);

  if ((in_memory_db == nullptr) || (result_code != SQLITE_OK)) {
    throw cpp::MessageException{"Couldn't create in memory DB: " +
                                std::to_string(result_code)};
  }

  DbFacade{logger_, cpp::AssumeNn(in_memory_db)}.EnableForeignKeys();

  return {cpp::AssumeNn(
      NullableSqliteDbHandle{in_memory_db, detail::SqliteDbCloser{logger_}})};
}

auto DbHandlesFactory::CreateHandleToFileDb(const FilePath &file_path) const
    -> SqliteDbHandle {
  Expects(!file_path.value.empty());

  auto *file_db = (sqlite3 *){};
  const auto result_code = sqlite3_open(file_path.value.c_str(), &file_db);

  if ((file_db == nullptr) || (result_code != SQLITE_OK)) {
    throw cpp::MessageException{std::string{"Couldn't read DB from file "} +
                                file_path.value + ": " +
                                std::to_string(result_code)};
  }

  return {cpp::AssumeNn(
      NullableSqliteDbHandle{file_db, detail::SqliteDbCloser{logger_}})};
}

auto DbHandlesFactory::LoadDbFromFileToMemory(const FilePath &file_path) const
    -> SqliteDbHandle {
  Expects(!file_path.value.empty());

  auto in_memory_db_handle = CreateInMemoryDb();

  if (const auto db_is_new = !std::filesystem::exists(file_path.value)) {
    logger_->LogImportantEvent(
        cpp::Format("Created new DB for {}", file_path.value));
    return in_memory_db_handle;
  }

  auto file_db_handle = CreateHandleToFileDb(file_path);
  DbFacade{logger_, cpp::AssumeNn(in_memory_db_handle.get())}.CopyDataFrom(
      *file_db_handle);

  logger_->LogImportantEvent(cpp::Format("Loaded DB from {}", file_path.value));
  return in_memory_db_handle;
}
}  // namespace stonks::sqlite