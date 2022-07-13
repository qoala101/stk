#include "sqlite_factory.h"

#include "not_null.hpp"
#include "sqlite_db.h"
#include "sqlite_query_builder.h"
#include "sqlite_utils.h"

namespace stonks::sqlite {
auto Factory::LoadDbFromFile(std::string_view file_path) const
    -> cpp::not_null<std::unique_ptr<sqldb::IDb>> {
  return cpp::assume_not_null(
      std::make_unique<Db>(utils::LoadSqliteDbFromFileToMemory(file_path)));
}

auto Factory::CreateQueryBuilder() const
    -> cpp::not_null<std::unique_ptr<sqldb::IQueryBuilder>> {
  return cpp::assume_not_null(std::make_unique<QueryBuilder>());
}
}  // namespace stonks::sqlite