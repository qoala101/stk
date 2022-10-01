#include "sqldb_query_builder_facade.h"

#include <utility>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "sqldb_i_query_builder.h"

namespace stonks::sqldb {
QueryBuilderFacade::QueryBuilderFacade(
    di::Factory<IQueryBuilder> query_builder_factory)
    : query_builder_factory_{std::move(query_builder_factory)} {}

auto QueryBuilderFacade::Select() const -> qbf::SelectQueryBuilder {
  return cpp::CallExposedPrivateConstructorOf<qbf::SelectQueryBuilder,
                                              QueryBuilderFacade>{}(
      query_builder_factory_.Create());
}

auto QueryBuilderFacade::Insert() const -> qbf::InsertQueryBuilder {
  return cpp::CallExposedPrivateConstructorOf<qbf::InsertQueryBuilder,
                                              QueryBuilderFacade>{}(
      query_builder_factory_.Create());
}

auto QueryBuilderFacade::Update() const -> qbf::UpdateQueryBuilder {
  return cpp::CallExposedPrivateConstructorOf<qbf::UpdateQueryBuilder,
                                              QueryBuilderFacade>{}(
      query_builder_factory_.Create());
}

auto QueryBuilderFacade::Delete() const -> qbf::DeleteQueryBuilder {
  return cpp::CallExposedPrivateConstructorOf<qbf::DeleteQueryBuilder,
                                              QueryBuilderFacade>{}(
      query_builder_factory_.Create());
}
}  // namespace stonks::sqldb