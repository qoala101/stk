#include "sqlite_factory.h"

#include "sqlite_db.h"
#include "sqlite_query_builder.h"
#include "sqlite_read_from_file.h"

namespace stonks::sqlite {
auto Factory::LoadDbFromFile(std::string_view file_path) const
    -> cpp::NnUp<sqldb::IDb> {
  return cpp::MakeNnUp<Db>(
      read_from_file::LoadSqliteDbFromFileToMemory(file_path));
}

auto Factory::CreateQueryBuilder() const -> cpp::NnUp<sqldb::IQueryBuilder> {
  return cpp::MakeNnUp<QueryBuilder>();
}
}  // namespace stonks::sqlite