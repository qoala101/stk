#include "sqldb_p_db.h"

#include <memory>
#include <type_traits>
#include <utility>

#include "cpp_not_null.h"
#include "not_null.hpp"
#include "sqldb_i_db.h"
#include "sqldb_p_select_statement.h"
#include "sqldb_p_update_statement.h"

namespace stonks::sqldb::p {
Db::Db(cpp::NnUp<IDb> db) : db_{std::move(db)} {}

auto Db::PrepareStatement(Parametrized<SelectQuery> query)
    -> cpp::NnUp<ISelectStatement> {
  return cpp::MakeNnUp<SelectStatement>(db_->PrepareStatement(std::move(query)),
                                        std::move(query.params));
}

auto Db::PrepareStatement(Parametrized<Query> query)
    -> cpp::NnUp<IUpdateStatement> {
  return cpp::MakeNnUp<UpdateStatement>(db_->PrepareStatement(std::move(query)),
                                        std::move(query.params));
}

auto Db::GetDbImpl(cpp::This<Db> auto &t) -> auto & { return *t.db_; }

auto Db::GetDb() const -> const IDb & { return GetDbImpl(*this); }

auto Db::GetDb() -> IDb & { return GetDbImpl(*this); }
}  // namespace stonks::sqldb::p