#include "sqlite_db_handle_variant.h"

#include <gsl/assert>
#include <variant>

#include "cpp_copy_const.h"
#include "sqlite_db_file_handle.h"

namespace stonks::sqlite {
template <cpp::DecaysTo<SqliteDbHandleVariant> This>
auto SqliteDbHandleVariant::GetSqliteDbImpl(This &t)
    -> cpp::CopyConst<This, sqlite3> & {
  return std::visit(
      [](auto &v) -> cpp::CopyConst<This, sqlite3> & {
        using V = decltype(v);

        if constexpr (cpp::DecaysTo<V, SqliteDbHandle>) {
          return *v;
        }

        if constexpr (cpp::DecaysTo<V, SqliteDbFileHandle>) {
          return v.GetSqliteDb();
        }

        Expects(false);
      },
      static_cast<
          cpp::CopyConst<decltype(t), detail::SqliteDbHandleVariantType> &>(t));
}

auto SqliteDbHandleVariant::GetSqliteDb() const -> const sqlite3 & {
  return GetSqliteDbImpl(*this);
}

auto SqliteDbHandleVariant::GetSqliteDb() -> sqlite3 & {
  return GetSqliteDbImpl(*this);
}

auto SqliteDbHandleVariant::GetFilePath() const -> const FilePath & {
  Expects(HasFilePath());
  return std::visit(
      [](const auto &v) -> const FilePath & {
        using V = decltype(v);

        if constexpr (cpp::DecaysTo<V, SqliteDbFileHandle>) {
          return v.GetFilePath();
        }

        Expects(false);
      },
      static_cast<const detail::SqliteDbHandleVariantType &>(*this));
}

auto SqliteDbHandleVariant::HasFilePath() const -> bool {
  return std::holds_alternative<SqliteDbFileHandle>(*this);
}
}  // namespace stonks::sqlite