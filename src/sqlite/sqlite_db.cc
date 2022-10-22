#include "sqlite_db.h"

#include <memory>
#include <not_null.hpp>
#include <utility>

#include "cpp_not_null.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqlite_db_facade.h"
#include "sqlite_db_handle_variant.h"
#include "sqlite_prepared_statement_handle.h"
#include "sqlite_ps_common_impl.h"
#include "sqlite_select_statement.h"
#include "sqlite_update_statement.h"

namespace stonks::sqlite {
Db::Db(di::Factory<log::ILogger> logger_factory,
       SqliteDbHandleVariant sqlite_db_handle)
    : logger_factory_{std::move(logger_factory)},
      sqlite_db_handle_{
          cpp::MakeNnSp<SqliteDbHandleVariant>(std::move(sqlite_db_handle))},
      sqlite_db_facade_{logger_factory_,
                        cpp::AssumeNn(&sqlite_db_handle_->GetSqliteDb())} {}

auto Db::PsCommonImplFrom(sqldb::Query query) const {
  return ps::CommonImpl{
      PreparedStatementHandle{sqlite_db_handle_,
                              sqlite_db_facade_.CreatePreparedStatement(query)},
      std::move(query), logger_factory_.Create()};
}

auto Db::PrepareStatement(sqldb::SelectQuery query)
    -> cpp::NnUp<sqldb::ISelectStatement> {
  return cpp::MakeNnUp<SelectStatement>(
      PsCommonImplFrom(std::move(query.query)),
      std::move(query.column_types));
}

auto Db::PrepareStatement(sqldb::Query query)
    -> cpp::NnUp<sqldb::IUpdateStatement> {
  return cpp::MakeNnUp<UpdateStatement>(PsCommonImplFrom(std::move(query)));
}
}  // namespace stonks::sqlite