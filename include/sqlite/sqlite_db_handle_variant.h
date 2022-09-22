#ifndef STONKS_SQLITE_SQLITE_DB_HANDLE_VARIANT_H_
#define STONKS_SQLITE_SQLITE_DB_HANDLE_VARIANT_H_

#include <variant>

#include "cpp_copy_const.h"
#include "sqlite_db_file_handle.h"
#include "sqlite_raw_handles.h"
#include "sqlite_types.h"

namespace stonks::sqlite {
namespace detail {
using SqliteDbHandleVariantType =
    std::variant<std::monostate, SqliteDbHandle, SqliteDbFileHandle>;
}  // namespace detail

/**
 * @brief Variant of SQLite DB handle which keeps connection alive.
 */
class SqliteDbHandleVariant : public detail::SqliteDbHandleVariantType {
 public:
  using detail::SqliteDbHandleVariantType::variant;

  /**
   * @brief Gives native SQLite handle.
   */
  [[nodiscard]] auto GetSqliteDb() const -> const sqlite3 &;

  /**
   * @copydoc GetSqliteDb
   */
  [[nodiscard]] auto GetSqliteDb() -> sqlite3 &;

  /**
   * @brief Gives DB file path.
   * @remark Should only be called if variant has file path.
   */
  [[nodiscard]] auto GetFilePath() const -> const FilePath &;

  /**
   * @brief Whether variant has file path.
   */
  [[nodiscard]] auto HasFilePath() const -> bool;

 private:
  template <cpp::DecaysTo<SqliteDbHandleVariant> This>
  [[nodiscard]] static auto GetSqliteDbImpl(This &t)
      -> cpp::CopyConst<This, sqlite3 &>;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_DB_HANDLE_VARIANT_H_
