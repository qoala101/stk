#include "sqlite_native_db_file_handle.h"

#include <exception>
#include <memory>
#include <not_null.hpp>
#include <utility>

#include "cpp_not_null.h"
#include "di_factory.h"
#include "sqlite_native_db_facade.h"

namespace stonks::sqlite {
NativeDbFileHandle::NativeDbFileHandle(di::Factory<log::ILogger> logger_factory,
                                       NativeDbHandle native_db_handle,
                                       FilePath file_path)
    : logger_factory_{std::move(logger_factory)},
      native_db_handle_{std::move(native_db_handle)},
      file_path_{std::move(file_path)} {}

NativeDbFileHandle::~NativeDbFileHandle() {
  auto* native_db = native_db_handle_.get();

  if (const auto object_was_moved = native_db == nullptr) {
    return;
  }

  auto logger = logger_factory_.Create();

  try {
    NativeDbFacade{std::move(logger_factory_)}.WriteToFile(*native_db,
                                                           file_path_);
  } catch (const std::exception& e) {
    logger->LogErrorCondition(e.what());
  }
}

auto NativeDbFileHandle::GetNativeDbImpl(cpp::This<NativeDbFileHandle> auto& t)
    -> auto& {
  return *t.native_db_handle_;
}

auto NativeDbFileHandle::GetNativeDb() const -> const sqlite3& {
  return GetNativeDbImpl(*this);
}

auto NativeDbFileHandle::GetNativeDb() -> sqlite3& {
  return GetNativeDbImpl(*this);
}

auto NativeDbFileHandle::GetFilePath() const -> const FilePath& {
  return file_path_;
}
}  // namespace stonks::sqlite