#ifndef VH_SQLITE_NATIVE_DB_HANDLES_FACTORY_H_
#define VH_SQLITE_NATIVE_DB_HANDLES_FACTORY_H_

#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "sqlite_native_handles.h"
#include "sqlite_types.h"

/**
 * @file Constructors of SQLite handles.
 */

namespace vh::sqlite {
class NativeDbHandlesFactory {
 public:
  explicit NativeDbHandlesFactory(cpp::NnSp<log::ILogger> logger);

  /**
   * @brief Creates in-memory SQLite DB.
   */
  auto CreateInMemoryDb [[nodiscard]] () const -> NativeDbHandle;

  /**
   * @brief Creates SQLite handle to DB from file.
   */
  auto CreateHandleToFileDb [[nodiscard]] (const FilePath &file_path) const
      -> NativeDbHandle;

  /**
   * @brief Reads SQLite DB from file and copies the contents to the in-memory
   * DB.
   * @return Handle to in-memory DB.
   */
  auto LoadDbFromFileToMemory [[nodiscard]] (const FilePath &file_path) const
      -> NativeDbHandle;

 private:
  void CreateParentDirectoryIfNotExists(const FilePath &file_path) const;

  cpp::NnSp<log::ILogger> logger_;
};
}  // namespace vh::sqlite

#endif  // VH_SQLITE_NATIVE_DB_HANDLES_FACTORY_H_
