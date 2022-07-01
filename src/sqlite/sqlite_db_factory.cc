#include "sqlite_db_factory.h"

#include "not_null.hpp"
#include "sqlite_db.h"
#include "sqlite_query_builder.h"
#include "sqlite_utils.h"

namespace stonks::sqlite {
auto SqliteDbFactory::LoadDbFromFile(std::string_view file_path) const
    -> cpp::not_null<std::unique_ptr<sqldb::IDb>> {
  return cpp::assume_not_null(std::make_unique<SqliteDb>(
      utils::LoadSqliteDbFromFileToMemory(file_path)));
}

auto SqliteDbFactory::CreateQueryBuilder() const
    -> cpp::not_null<std::unique_ptr<sqldb::IQueryBuilder>> {
  return cpp::assume_not_null(std::make_unique<SqliteQueryBuilder>());
}
}  // namespace stonks::sqlite