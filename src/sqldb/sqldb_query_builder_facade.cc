#include "sqldb_query_builder_facade.h"

#include <memory>
#include <utility>

#include "ccutils_expose_private_constructors.h"
#include "not_null.hpp"
#include "sqldb_i_query_builder.h"

namespace stonks::sqldb {
QueryBuilderFacade::QueryBuilderFacade(
    cpp::not_null<std::shared_ptr<IQueryBuilder>> query_builder)
    : query_builder_{std::move(query_builder)} {}

[[nodiscard]] auto QueryBuilderFacade::Select() const
    -> query_builder_facade::SelectQueryBuilder {
  return ccutils::CallExposedPrivateConstructorOf<
      query_builder_facade::SelectQueryBuilder, QueryBuilderFacade>{}(
      query_builder_);
}

[[nodiscard]] auto QueryBuilderFacade::Insert() const
    -> query_builder_facade::InsertQueryBuilder {
  return ccutils::CallExposedPrivateConstructorOf<
      query_builder_facade::InsertQueryBuilder, QueryBuilderFacade>{}(
      query_builder_);
}

[[nodiscard]] auto QueryBuilderFacade::Update() const
    -> query_builder_facade::UpdateQueryBuilder {
  return ccutils::CallExposedPrivateConstructorOf<
      query_builder_facade::UpdateQueryBuilder, QueryBuilderFacade>{}(
      query_builder_);
}

[[nodiscard]] auto QueryBuilderFacade::Delete() const
    -> query_builder_facade::DeleteQueryBuilder {
  return ccutils::CallExposedPrivateConstructorOf<
      query_builder_facade::DeleteQueryBuilder, QueryBuilderFacade>{}(
      query_builder_);
}
}  // namespace stonks::sqldb