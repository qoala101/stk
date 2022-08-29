#include "sqlite_db.h"

#include <utility>

#include "cpp_not_null.h"
#include "sqldb_enums_to_string.h"  // IWYU pragma: keep
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqlite_db_facade.h"
#include "sqlite_prepared_statement_handle.h"
#include "sqlite_select_statement.h"
#include "sqlite_update_statement.h"

namespace stonks::sqlite {
Db::Db(SqliteDbSharedHandle sqlite_db_handle)
    : sqlite_db_handle_{std::move(sqlite_db_handle)},
      sqlite_db_facade_{cpp::AssumeNn(sqlite_db_handle_.get())} {}

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

void Db::WriteToFile(std::string_view file_path) const {
  sqlite_db_facade_.WriteToFile(file_path);
}
}  // namespace stonks::sqlite