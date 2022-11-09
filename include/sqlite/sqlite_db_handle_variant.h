#ifndef STONKS_SQLITE_SQLITE_DB_HANDLE_VARIANT_H_
#define STONKS_SQLITE_SQLITE_DB_HANDLE_VARIANT_H_

#include <variant>

#include "cpp_this.h"  // IWYU pragma: keep
#include "cpp_variant_struct.h"
#include "sqlite_db_file_handle.h"
#include "sqlite_raw_handles.h"
#include "sqlite_types.h"

namespace stonks::sqlite {
/**
 * @brief Variant of SQLite DB handle which keeps connection alive.
 */
struct SqliteDbHandleVariant
    : public cpp::VariantStruct<std::monostate, SqliteDbHandle,
                                SqliteDbFileHandle> {
 public:
  /**
   * @brief Gives native SQLite handle.
   */
  auto GetSqliteDb [[nodiscard]] () const -> const sqlite3 &;
  auto GetSqliteDb [[nodiscard]] () -> sqlite3 &;

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
  static auto GetSqliteDbImpl
      [[nodiscard]] (cpp::This<SqliteDbHandleVariant> auto &t) -> auto &;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_DB_HANDLE_VARIANT_H_
