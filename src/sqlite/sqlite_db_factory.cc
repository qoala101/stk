#include "sqlite_db_factory.h"

#include "sqlite_db.h"
#include "sqlite_query_builder.h"
#include "sqlite_utils.h"

namespace stonks::sqlite {
auto SqliteDbFactory::LoadDbFromFile(std::string_view file_path)
    -> std::unique_ptr<sqldb::Db> {
  return std::make_unique<SqliteDb>(utils::ReadSqliteDbFromFile(file_path));
}

auto SqliteDbFactory::CreateQueryBuilder()
    -> std::unique_ptr<sqldb::QueryBuilder> {
  return std::make_unique<SqliteQueryBuilder>();
}
}  // namespace stonks::sqlite