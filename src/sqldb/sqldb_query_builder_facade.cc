#include "sqldb_query_builder_facade.h"

#include <utility>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "sqldb_i_query_builder.h"

namespace stonks::sqldb {
QueryBuilderFacade::QueryBuilderFacade(cpp::NnSp<IQueryBuilder> query_builder)
    : query_builder_{std::move(query_builder)} {}

auto QueryBuilderFacade::Select() const -> qbf::SelectQueryBuilder {
  return cpp::CallExposedPrivateConstructorOf<qbf::SelectQueryBuilder,
                                              QueryBuilderFacade>{}(
      query_builder_);
}

auto QueryBuilderFacade::Insert() const -> qbf::InsertQueryBuilder {
  return cpp::CallExposedPrivateConstructorOf<qbf::InsertQueryBuilder,
                                              QueryBuilderFacade>{}(
      query_builder_);
}

auto QueryBuilderFacade::Update() const -> qbf::UpdateQueryBuilder {
  return cpp::CallExposedPrivateConstructorOf<qbf::UpdateQueryBuilder,
                                              QueryBuilderFacade>{}(
      query_builder_);
}

auto QueryBuilderFacade::Delete() const -> qbf::DeleteQueryBuilder {
  return cpp::CallExposedPrivateConstructorOf<qbf::DeleteQueryBuilder,
                                              QueryBuilderFacade>{}(
      query_builder_);
}
}  // namespace stonks::sqldb