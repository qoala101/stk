#include "sqlite_native_db_handles_factory.h"

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
#include "sqlite_native_db_facade.h"
#include "sqlite_native_handles.h"

namespace vh::sqlite {
namespace {
using NullableNativeDbHandle =
    std::remove_cvref_t<decltype(std::declval<NativeDbHandle>().as_nullable())>;
}  // namespace

NativeDbHandlesFactory::NativeDbHandlesFactory(cpp::NnSp<log::ILogger> logger)
    : logger_{std::move(logger)} {}

auto NativeDbHandlesFactory::CreateInMemoryDb() const -> NativeDbHandle {
  auto *in_memory_db = static_cast<sqlite3 *>(nullptr);
  const auto result_code = sqlite3_open(":memory:", &in_memory_db);

  if ((in_memory_db == nullptr) || (result_code != SQLITE_OK)) {
    throw cpp::MessageException{
        fmt::format("Couldn't create in memory DB: {}", result_code)};
  }

  NativeDbFacade::EnableForeignKeys(*in_memory_db);

  return {cpp::AssumeNn(
      NullableNativeDbHandle{in_memory_db, detail::NativeDbCloser{logger_}})};
}

auto NativeDbHandlesFactory::CreateHandleToFileDb(
    const FilePath &file_path) const -> NativeDbHandle {
  Expects(!file_path->empty());

  CreateParentDirectoryIfNotExists(file_path);

  auto *file_db = static_cast<sqlite3 *>(nullptr);
  const auto result_code = sqlite3_open(file_path->c_str(), &file_db);

  if ((file_db == nullptr) || (result_code != SQLITE_OK)) {
    throw cpp::MessageException{fmt::format("Couldn't read DB from file {}: {}",
                                            *file_path, result_code)};
  }

  return {cpp::AssumeNn(
      NullableNativeDbHandle{file_db, detail::NativeDbCloser{logger_}})};
}

auto NativeDbHandlesFactory::LoadDbFromFileToMemory(
    const FilePath &file_path) const -> NativeDbHandle {
  Expects(!file_path->empty());

  auto in_memory_db_handle = CreateInMemoryDb();

  if (const auto db_is_new = !std::filesystem::exists(*file_path)) {
    logger_->LogImportantEvent(
        fmt::format("Created new DB for {}", *file_path));
    return in_memory_db_handle;
  }

  auto file_db_handle = CreateHandleToFileDb(file_path);
  NativeDbFacade::CopyDataFrom(*in_memory_db_handle, *file_db_handle);

  logger_->LogImportantEvent(fmt::format("Loaded DB from {}", *file_path));
  return in_memory_db_handle;
}

void NativeDbHandlesFactory::CreateParentDirectoryIfNotExists(
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
}  // namespace vh::sqlite