#include "sqldb_query_builder_facade.h"

#include <utility>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "sqldb_i_query_builder.h"

namespace stonks::sqldb {
QueryBuilderFacade::QueryBuilderFacade(
    cpp::NnSp<di::IFactory<IQueryBuilder>> query_builder_factory)
    : query_builder_factory_{std::move(query_builder_factory)} {}

auto QueryBuilderFacade::Select() const -> qbf::SelectQueryBuilder {
  return cpp::CallExposedPrivateConstructorOf<qbf::SelectQueryBuilder,
                                              QueryBuilderFacade>{}(
      cpp::AssumeNn(query_builder_factory_->create()));
}

auto QueryBuilderFacade::Insert() const -> qbf::InsertQueryBuilder {
  return cpp::CallExposedPrivateConstructorOf<qbf::InsertQueryBuilder,
                                              QueryBuilderFacade>{}(
      cpp::AssumeNn(query_builder_factory_->create()));
}

auto QueryBuilderFacade::Update() const -> qbf::UpdateQueryBuilder {
  return cpp::CallExposedPrivateConstructorOf<qbf::UpdateQueryBuilder,
                                              QueryBuilderFacade>{}(
      cpp::AssumeNn(query_builder_factory_->create()));
}

auto QueryBuilderFacade::Delete() const -> qbf::DeleteQueryBuilder {
  return cpp::CallExposedPrivateConstructorOf<qbf::DeleteQueryBuilder,
                                              QueryBuilderFacade>{}(
      cpp::AssumeNn(query_builder_factory_->create()));
}
}  // namespace stonks::sqldb