#include "sqlite_db_handles_factory.h"

#include <fmt/core.h>
#include <sqlite3.h>

#include <filesystem>
#include <gsl/assert>
#include <memory>
#include <not_null.hpp>
#include <string>
#include <type_traits>
#include <utility>

#include "cpp_message_exception.h"
#include "cpp_not_null.h"
#include "cpp_typed_struct.h"
#include "sqlite_db_facade.h"
#include "sqlite_raw_handles.h"

namespace stonks::sqlite {
namespace {
using NullableSqliteDbHandle =
    std::remove_cvref_t<decltype(std::declval<SqliteDbHandle>().as_nullable())>;
}  // namespace

DbHandlesFactory::DbHandlesFactory(di::Factory<log::ILogger> logger_factory)
    : logger_factory_{std::move(logger_factory)},
      logger_{logger_factory_.Create()} {}

auto DbHandlesFactory::CreateInMemoryDb() const -> SqliteDbHandle {
  auto *in_memory_db = static_cast<sqlite3 *>(nullptr);
  const auto result_code = sqlite3_open(":memory:", &in_memory_db);

  if ((in_memory_db == nullptr) || (result_code != SQLITE_OK)) {
    throw cpp::MessageException{
        fmt::format("Couldn't create in memory DB: {}", result_code)};
  }

  DbFacade{logger_factory_, cpp::AssumeNn(in_memory_db)}.EnableForeignKeys();

  return {cpp::AssumeNn(NullableSqliteDbHandle{
      in_memory_db, detail::SqliteDbCloser{logger_factory_}})};
}

auto DbHandlesFactory::CreateHandleToFileDb(const FilePath &file_path) const
    -> SqliteDbHandle {
  Expects(!file_path->empty());

  CreateParentDirectoryIfNotExists(file_path);

  auto *file_db = static_cast<sqlite3 *>(nullptr);
  const auto result_code = sqlite3_open(file_path->c_str(), &file_db);

  if ((file_db == nullptr) || (result_code != SQLITE_OK)) {
    throw cpp::MessageException{fmt::format("Couldn't read DB from file {}: {}",
                                            *file_path, result_code)};
  }

  return {cpp::AssumeNn(NullableSqliteDbHandle{
      file_db, detail::SqliteDbCloser{logger_factory_}})};
}

auto DbHandlesFactory::LoadDbFromFileToMemory(const FilePath &file_path) const
    -> SqliteDbHandle {
  Expects(!file_path->empty());

  auto in_memory_db_handle = CreateInMemoryDb();

  if (const auto db_is_new = !std::filesystem::exists(*file_path)) {
    logger_->LogImportantEvent(
        fmt::format("Created new DB for {}", *file_path));
    return in_memory_db_handle;
  }

  auto file_db_handle = CreateHandleToFileDb(file_path);
  DbFacade{logger_factory_, cpp::AssumeNn(in_memory_db_handle.get())}
      .CopyDataFrom(*file_db_handle);

  logger_->LogImportantEvent(fmt::format("Loaded DB from {}", *file_path));
  return in_memory_db_handle;
}

void DbHandlesFactory::CreateParentDirectoryIfNotExists(
    const FilePath &file_path) const {
  const auto directory_path = std::filesystem::path{*file_path}.parent_path();

  if (const auto file_in_current_dir = directory_path.empty()) {
    return;
  }

  const auto directory_path_created =
      std::filesystem::create_directories(directory_path);

  if (directory_path_created) {
    logger_->LogImportantEvent(
        fmt::format("Created directory path to store DB {}", *file_path));
  }
}
}  // namespace stonks::sqlite