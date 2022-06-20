#include "sqlite_prepared_statement_handle.h"

#include <sqlite3.h>

#include <gsl/assert>
#include <utility>

namespace stonks::sqlite {
SqlitePreparedStatementHandle::SqlitePreparedStatementHandle(
    std::weak_ptr<sqlite3_stmt> sqlite_statement,
    std::function<void(sqlite3_stmt &)>
        sqlite_statement_handle_deleted_callback)
    : sqlite_statement_{std::move(sqlite_statement)},
      sqlite_statement_handle_deleted_callback_{
          std::move(sqlite_statement_handle_deleted_callback)} {
  Expects(!sqlite_statement_.expired());
  Expects(sqlite_statement_handle_deleted_callback_);
}

SqlitePreparedStatementHandle::~SqlitePreparedStatementHandle() {
  if (const auto sqlite_statement = sqlite_statement_.lock()) {
    sqlite_statement_handle_deleted_callback_(*sqlite_statement);
  }
}

auto SqlitePreparedStatementHandle::GetSqliteStatement() const
    -> std::shared_ptr<sqlite3_stmt> {
  return sqlite_statement_.lock();
}

auto SqlitePreparedStatementHandle::IsValid() const -> bool {
  return !sqlite_statement_.expired();
}
}  // namespace stonks::sqlite