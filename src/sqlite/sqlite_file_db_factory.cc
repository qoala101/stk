#include "sqlite_file_db_factory.h"

#include <utility>

#include "cpp_not_null.h"
#include "sqlite_db.h"
#include "sqlite_db_file_handle.h"
#include "sqlite_db_handle_variant.h"
#include "sqlite_db_handles_factory.h"
#include "sqlite_query_builder.h"

namespace stonks::sqlite {
FileDbFactory::FileDbFactory(std::string db_file_path)
    : db_file_path_{std::move(db_file_path)} {}

auto FileDbFactory::CreateDb() const -> cpp::NnUp<sqldb::IDb> {
  auto file_handle = SqliteDbFileHandle{
      db_handles_factory::LoadDbFromFileToMemory(db_file_path_), db_file_path_};
  return cpp::MakeNnUp<Db>(
      cpp::MakeNnSp<SqliteDbHandleVariant>(std::move(file_handle)));
}

auto FileDbFactory::CreateQueryBuilder() const
    -> cpp::NnUp<sqldb::IQueryBuilder> {
  return cpp::MakeNnUp<QueryBuilder>();
}
}  // namespace stonks::sqlite