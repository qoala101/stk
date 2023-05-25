/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_SQLITE_NATIVE_DB_HANDLE_VARIANT_H_
#define VH_SQLITE_NATIVE_DB_HANDLE_VARIANT_H_

#include <variant>

#include "cpp_this.h"  // IWYU pragma: keep
#include "cpp_variant_struct.h"
#include "sqlite_native_db_file_handle.h"
#include "sqlite_native_handles.h"
#include "sqlite_types.h"

namespace vh::sqlite {
/**
 * @brief Variant of SQLite DB handle which keeps connection alive.
 */
struct NativeDbHandleVariant
    : public cpp::VariantStruct<NativeDbHandle, NativeDbFileHandle> {
 public:
  /**
   * @brief Gives native SQLite handle.
   */
  auto GetNativeDb [[nodiscard]] () const -> const sqlite3 &;
  auto GetNativeDb [[nodiscard]] () -> sqlite3 &;

  /**
   * @brief Gives DB file path.
   * @remark Should only be called if variant has file path.
   */
  auto GetFilePath [[nodiscard]] () const -> const FilePath &;

  /**
   * @brief Whether variant has file path.
   */
  auto HasFilePath [[nodiscard]] () const -> bool;

 private:
  static auto GetNativeDbImpl
      [[nodiscard]] (cpp::This<NativeDbHandleVariant> auto &t) -> auto &;
};
}  // namespace vh::sqlite

#endif  // VH_SQLITE_NATIVE_DB_HANDLE_VARIANT_H_
