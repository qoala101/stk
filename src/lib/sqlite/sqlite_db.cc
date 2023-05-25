/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "sqlite_db.h"

#include <memory>
#include <not_null.hpp>
#include <utility>

#include "cpp_not_null.h"
#include "cpp_share.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_types.h"
#include "sqlite_native_db_facade.h"
#include "sqlite_native_db_handle_variant.h"
#include "sqlite_prepared_statement_impl.h"
#include "sqlite_select_statement.h"
#include "sqlite_update_statement.h"

namespace vh::sqlite {
Db::Db(cpp::NnUp<log::ILogger> logger, NativeDbHandleVariant native_db_handle,
       cpp::Factory<cpp::MutexVariant> prepared_statement_mutex_factory)
    : logger_{std::move(logger)},
      native_db_handle_{cpp::Share(std::move(native_db_handle))},
      prepared_statement_mutex_factory_{
          std::move(prepared_statement_mutex_factory)},
      native_db_facade_{logger_} {}

auto Db::PreparedStatementImplFrom(sqldb::Query query) {
  auto native_statement_handle = native_db_facade_.CreatePreparedStatement(
      native_db_handle_->GetNativeDb(), query);
  auto native_statement_mutex = prepared_statement_mutex_factory_.Create();
  return PreparedStatementImpl{
      native_db_handle_, std::move(native_statement_handle), std::move(query),
      logger_, std::move(native_statement_mutex)};
}

auto Db::PrepareStatement(sqldb::SelectQuery query)
    -> cpp::NnUp<sqldb::ISelectStatement> {
  auto result_definition = std::move(query.result_definition);
  return cpp::MakeNnUp<SelectStatement>(
      PreparedStatementImplFrom(std::move(query)),
      std::move(result_definition));
}

auto Db::PrepareStatement(sqldb::Query query)
    -> cpp::NnUp<sqldb::IUpdateStatement> {
  return cpp::MakeNnUp<UpdateStatement>(
      PreparedStatementImplFrom(std::move(query)));
}
}  // namespace vh::sqlite