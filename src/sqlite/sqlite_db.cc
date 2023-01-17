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
#include "sqlite_ps_common_impl.h"
#include "sqlite_select_statement.h"
#include "sqlite_update_statement.h"

namespace stonks::sqlite {
Db::Db(di::Factory<log::ILogger> logger_factory,
       NativeDbHandleVariant native_db_handle)
    : logger_factory_{std::move(logger_factory)},
      native_db_handle_{cpp::Share(std::move(native_db_handle))},
      native_db_facade_{logger_factory_,
                        cpp::AssumeNn(&native_db_handle_->GetNativeDb())} {}

auto Db::PsCommonImplFrom(sqldb::Query query) const {
  return ps::CommonImpl{native_db_handle_,
                        native_db_facade_.CreatePreparedStatement(query),
                        std::move(query), logger_factory_.Create()};
}

auto Db::PrepareStatement(sqldb::SelectQuery query)
    -> cpp::NnUp<sqldb::ISelectStatement> {
  const auto result_definition = query.result_definition;
  return cpp::MakeNnUp<SelectStatement>(PsCommonImplFrom(std::move(query)),
                                        result_definition);
}

auto Db::PrepareStatement(sqldb::Query query)
    -> cpp::NnUp<sqldb::IUpdateStatement> {
  return cpp::MakeNnUp<UpdateStatement>(PsCommonImplFrom(std::move(query)));
}
}  // namespace stonks::sqlite