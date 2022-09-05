#include "sqlite_db_handle_variant.h"

#include <gsl/assert>
#include <utility>

#include "cpp_copy_const.h"
#include "not_null.hpp"

namespace stonks::sqlite {
SqliteDbHandleVariant::SqliteDbHandleVariant(SqliteDbHandle sqlite_db_handle)
    : sqlite_db_handle_{std::move(sqlite_db_handle)} {}

SqliteDbHandleVariant::SqliteDbHandleVariant(
    SqliteDbFileHandle sqlite_db_handle)
    : sqlite_db_handle_{std::move(sqlite_db_handle)} {}

auto SqliteDbHandleVariant::GetSqliteDbImpl(
    cpp::DecaysTo<SqliteDbHandleVariant> auto &&t)
    -> cpp::CopyConst<decltype(t), sqlite3 &> {
  using T = decltype(t);
  auto &&ft = std::forward<T>(t);

  return std::visit(
      [](auto &&v) -> cpp::CopyConst<T, sqlite3 &> {
        using V = decltype(v);
        auto &&fv = std::forward<V>(v);

        if constexpr (cpp::DecaysTo<V, SqliteDbHandle>) {
          return *fv;
        }

        if constexpr (cpp::DecaysTo<V, SqliteDbFileHandle>) {
          return fv.GetSqliteDb();
        }

        Expects(false);
      },
      ft.sqlite_db_handle_);
}

auto SqliteDbHandleVariant::GetSqliteDb() const -> const sqlite3 & {
  return GetSqliteDbImpl(*this);
}

auto SqliteDbHandleVariant::GetSqliteDb() -> sqlite3 & {
  return GetSqliteDbImpl(*this);
}

auto SqliteDbHandleVariant::GetFilePath() const -> const std::string * {
  return std::visit(
      [](auto &&v) -> const std::string * {
        using V = decltype(v);
        auto &&fv = std::forward<V>(v);

        if constexpr (cpp::DecaysTo<V, SqliteDbHandle>) {
          return nullptr;
        }

        if constexpr (cpp::DecaysTo<V, SqliteDbFileHandle>) {
          return &fv.GetFilePath();
        }

        Expects(false);
      },
      sqlite_db_handle_);
}
}  // namespace stonks::sqlite