#include "sqldb_query_builder_facade.h"

#include <utility>

namespace stonks::sqldb {
QueryBuilderFacade::QueryBuilderFacade(
    std::shared_ptr<IQueryBuilder> query_builder)
    : query_builder_(std::move(query_builder)) {}

[[nodiscard]] auto QueryBuilderFacade::Select() -> SelectQueryBuilder {
  return SelectQueryBuilder(query_builder_);
}

[[nodiscard]] auto QueryBuilderFacade::Insert() -> InsertQueryBuilder {
  return InsertQueryBuilder(query_builder_);
}

[[nodiscard]] auto QueryBuilderFacade::Update() -> UpdateQueryBuilder {
  return UpdateQueryBuilder(query_builder_);
}

[[nodiscard]] auto QueryBuilderFacade::Delete() -> DeleteQueryBuilder {
  return DeleteQueryBuilder(query_builder_);
}
}  // namespace stonks::sqldb