#include "sqlite_db.h"

#include <memory>
#include <not_null.hpp>
#include <utility>

#include "cpp_not_null.h"
#include "cpp_share.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqlite_native_db_facade.h"
#include "sqlite_native_db_handle_variant.h"
#include "sqlite_prepared_statement_impl.h"
#include "sqlite_select_statement.h"
#include "sqlite_update_statement.h"

namespace stonks::sqlite {
Db::Db(cpp::NnUp<log::ILogger> logger, NativeDbHandleVariant native_db_handle)
    : logger_{std::move(logger)},
      native_db_handle_{cpp::Share(std::move(native_db_handle))},
      native_db_facade_{logger_} {}

auto Db::PreparedStatementImplFrom(sqldb::Query query) const {
  return PreparedStatementImpl{native_db_handle_,
                               native_db_facade_.CreatePreparedStatement(
                                   native_db_handle_->GetNativeDb(), query),
                               std::move(query), logger_};
}

auto Db::PrepareStatement(sqldb::SelectQuery query)
    -> cpp::NnUp<sqldb::ISelectStatement> {
  const auto result_definition = query.result_definition;
  return cpp::MakeNnUp<SelectStatement>(
      PreparedStatementImplFrom(std::move(query)), result_definition);
}

auto Db::PrepareStatement(sqldb::Query query)
    -> cpp::NnUp<sqldb::IUpdateStatement> {
  return cpp::MakeNnUp<UpdateStatement>(
      PreparedStatementImplFrom(std::move(query)));
}
}  // namespace stonks::sqlite