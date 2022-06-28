#include "sqldb_query_builder_facade.h"

#include <utility>

namespace stonks::sqldb {
QueryBuilderFacade::QueryBuilderFacade(
    std::shared_ptr<IQueryBuilder> query_builder)
    : query_builder_(std::move(query_builder)) {}

[[nodiscard]] auto QueryBuilderFacade::Select() const -> SelectQueryBuilder {
  return SelectQueryBuilder(query_builder_);
}

[[nodiscard]] auto QueryBuilderFacade::Insert() const -> InsertQueryBuilder {
  return InsertQueryBuilder(query_builder_);
}

[[nodiscard]] auto QueryBuilderFacade::Update() const -> UpdateQueryBuilder {
  return UpdateQueryBuilder(query_builder_);
}

[[nodiscard]] auto QueryBuilderFacade::Delete() const -> DeleteQueryBuilder {
  return DeleteQueryBuilder(query_builder_);
}
}  // namespace stonks::sqldb