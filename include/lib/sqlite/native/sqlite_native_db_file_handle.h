#ifndef STONKS_SQLITE_NATIVE_SQLITE_NATIVE_DB_FILE_HANDLE_H_
#define STONKS_SQLITE_NATIVE_SQLITE_NATIVE_DB_FILE_HANDLE_H_

#include "cpp_this.h"  // IWYU pragma: keep
#include "log_i_logger.h"
#include "sqlite_native_handles.h"
#include "sqlite_types.h"

namespace vh::sqlite {
/**
 * @remark Closes DB connection and writes DB to the file when destroyed.
 */
class NativeDbFileHandle {
 public:
  /**
   * @param file_path File from which SQLite DB was read
   * and where it's to be written.
   * @param native_db_handle Handle to DB on which to operate and write to file.
   */
  NativeDbFileHandle(cpp::NnUp<log::ILogger> logger,
                     NativeDbHandle native_db_handle, FilePath file_path);

  NativeDbFileHandle(const NativeDbFileHandle &) = delete;
  NativeDbFileHandle(NativeDbFileHandle &&) noexcept = default;

  auto operator=(const NativeDbFileHandle &) -> NativeDbFileHandle & = delete;
  auto operator=(NativeDbFileHandle &&) noexcept
      -> NativeDbFileHandle & = default;

  /**
   * @brief Closes SQLite DB and writes the changes to file.
   */
  ~NativeDbFileHandle();

  /**
   * @brief Gives native SQLite handle.
   */
  auto GetNativeDb [[nodiscard]] () const -> const sqlite3 &;
  auto GetNativeDb [[nodiscard]] () -> sqlite3 &;

  /**
   * @brief Gives file path DB was read from and would be stored to
   * when handle is destroyed.
   */
  auto GetFilePath [[nodiscard]] () const -> const FilePath &;

 private:
  static auto GetNativeDbImpl
      [[nodiscard]] (cpp::This<NativeDbFileHandle> auto &t) -> auto &;

  cpp::NnSp<log::ILogger> logger_;
  NativeDbHandle native_db_handle_;
  FilePath file_path_{};
};
}  // namespace vh::sqlite

#endif  // STONKS_SQLITE_NATIVE_SQLITE_NATIVE_DB_FILE_HANDLE_H_
