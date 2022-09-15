#include "sqlite_db.h"

#include <memory>
#include <utility>

#include "cpp_not_null.h"
#include "not_null.hpp"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqlite_db_facade.h"
#include "sqlite_db_handle_variant.h"
#include "sqlite_prepared_statement_handle.h"
#include "sqlite_select_statement.h"
#include "sqlite_update_statement.h"

namespace stonks::sqlite {
Db::Db(cpp::NnSp<log::ILogger> logger,
       cpp::NnSp<SqliteDbHandleVariant> sqlite_db_handle)
    : sqlite_db_handle_{std::move(sqlite_db_handle)},
      sqlite_db_facade_{std::move(logger),
                        cpp::AssumeNn(&sqlite_db_handle_->GetSqliteDb())} {}

auto Db::PrepareStatement(std::string query,
                          sqldb::RowDefinition result_definition)
    -> cpp::NnUp<sqldb::ISelectStatement> {
  return cpp::MakeNnUp<SelectStatement>(
      PreparedStatementHandle{sqlite_db_handle_,
                              sqlite_db_facade_.CreatePreparedStatement(query)},
      result_definition);
}

auto Db::PrepareStatement(std::string query)
    -> cpp::NnUp<sqldb::IUpdateStatement> {
  return cpp::MakeNnUp<UpdateStatement>(PreparedStatementHandle{
      sqlite_db_handle_, sqlite_db_facade_.CreatePreparedStatement(query)});
}
}  // namespace stonks::sqlite