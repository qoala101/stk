#include "sqlite_db_factory.h"

#include <gsl/assert>

#include "sqlite_db.h"
#include "sqlite_query_builder.h"
#include "sqlite_utils.h"

namespace stonks::sqlite {
auto SqliteDbFactory::LoadDbFromFile(std::string_view file_path) const
    -> std::unique_ptr<sqldb::IDb> {
  auto db = std::make_unique<SqliteDb>(
      utils::LoadSqliteDbFromFileToMemory(file_path));

  Ensures(db != nullptr);
  return db;
}

auto SqliteDbFactory::CreateQueryBuilder() const
    -> std::unique_ptr<sqldb::IQueryBuilder> {
  auto query_builder = std::make_unique<SqliteQueryBuilder>();

  Ensures(query_builder != nullptr);
  return query_builder;
}
}  // namespace stonks::sqlite