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

template <cpp::DecaysTo<SqliteDbHandleVariant> This>
auto SqliteDbHandleVariant::GetSqliteDbImpl(This &t)
    -> cpp::CopyConst<This, sqlite3 &> {
  return std::visit(
      [](auto &v) -> cpp::CopyConst<This, sqlite3 &> {
        using V = decltype(v);

        if constexpr (cpp::DecaysTo<V, SqliteDbHandle>) {
          return *v;
        }

        if constexpr (cpp::DecaysTo<V, SqliteDbFileHandle>) {
          return v.GetSqliteDb();
        }

        Expects(false);
      },
      t.sqlite_db_handle_);
}

auto SqliteDbHandleVariant::GetSqliteDb() const -> const sqlite3 & {
  return GetSqliteDbImpl(*this);
}

auto SqliteDbHandleVariant::GetSqliteDb() -> sqlite3 & {
  return GetSqliteDbImpl(*this);
}

auto SqliteDbHandleVariant::GetFilePath() const -> const std::string * {
  return std::visit(
      [](const auto &v) -> const std::string * {
        using V = decltype(v);

        if constexpr (cpp::DecaysTo<V, SqliteDbHandle>) {
          return nullptr;
        }

        if constexpr (cpp::DecaysTo<V, SqliteDbFileHandle>) {
          return &v.GetFilePath();
        }

        Expects(false);
      },
      sqlite_db_handle_);
}
}  // namespace stonks::sqlite