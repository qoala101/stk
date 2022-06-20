#include "sqlite_db_factory.h"

#include <gsl/assert>

#include "sqlite_db.h"
#include "sqlite_db_read_write_facade.h"
#include "sqlite_query_builder.h"

namespace stonks::sqlite {
auto SqliteDbFactory::LoadDbFromFile(std::string_view file_path)
    -> std::unique_ptr<sqldb::IDb> {
  auto db = std::make_unique<SqliteDb>(
      SqliteDbReadWriteFacade::ReadSqliteDbFromFile(file_path));

  Ensures(db != nullptr);
  return db;
}

auto SqliteDbFactory::CreateQueryBuilder()
    -> std::unique_ptr<sqldb::IQueryBuilder> {
  auto query_builder = std::make_unique<SqliteQueryBuilder>();
  
  Ensures(query_builder != nullptr);
  return query_builder;
}
}  // namespace stonks::sqlite