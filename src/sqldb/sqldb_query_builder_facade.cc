#include "sqldb_query_builder_facade.h"

#include <utility>

#include "ccutils_expose_private_constructors.h"
#include "ccutils_not_null.h"
#include "sqldb_i_query_builder.h"

namespace stonks::sqldb {
QueryBuilderFacade::QueryBuilderFacade(
    ccutils::NnSp<IQueryBuilder> query_builder)
    : query_builder_{std::move(query_builder)} {}

auto QueryBuilderFacade::Select() const
    -> query_builder_facade::SelectQueryBuilder {
  return ccutils::CallExposedPrivateConstructorOf<
      query_builder_facade::SelectQueryBuilder, QueryBuilderFacade>{}(
      query_builder_);
}

auto QueryBuilderFacade::Insert() const
    -> query_builder_facade::InsertQueryBuilder {
  return ccutils::CallExposedPrivateConstructorOf<
      query_builder_facade::InsertQueryBuilder, QueryBuilderFacade>{}(
      query_builder_);
}

auto QueryBuilderFacade::Update() const
    -> query_builder_facade::UpdateQueryBuilder {
  return ccutils::CallExposedPrivateConstructorOf<
      query_builder_facade::UpdateQueryBuilder, QueryBuilderFacade>{}(
      query_builder_);
}

auto QueryBuilderFacade::Delete() const
    -> query_builder_facade::DeleteQueryBuilder {
  return ccutils::CallExposedPrivateConstructorOf<
      query_builder_facade::DeleteQueryBuilder, QueryBuilderFacade>{}(
      query_builder_);
}
}  // namespace stonks::sqldb