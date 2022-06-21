#include "sqldb_select_query_builder.h"

#include <gsl/assert>
#include <type_traits>
#include <utility>
#include <variant>

#include "sqldb_query_builders_common.h"

namespace stonks::sqldb {
SelectQueryBuilder::SelectQueryBuilder(
    std::shared_ptr<IQueryBuilder> query_builder)
    : query_builder_{std::move(query_builder)} {
  Expects(query_builder_ != nullptr);
}

auto SelectQueryBuilder::Columns(std::vector<Column> columns)
    -> SelectQueryBuilder & {
  Expects(std::holds_alternative<std::monostate>(columns_));
  columns_.emplace<std::vector<Column>>(std::move(columns));
  return *this;
}

auto SelectQueryBuilder::AllColumns() -> SelectQueryBuilder & {
  Expects(std::holds_alternative<std::monostate>(columns_));
  columns_.emplace<AllColumnsType>();
  return *this;
}

auto SelectQueryBuilder::FromTable(Table table) -> SelectQueryBuilder & {
  Expects(!table_.has_value());
  table_.emplace(std::move(table));
  return *this;
}

auto SelectQueryBuilder::Where(std::string_view where_clause)
    -> SelectQueryBuilder & {
  Expects(!where_clause_.has_value());
  where_clause_.emplace(std::string{"WHERE "} + where_clause.data());
  return *this;
}

auto SelectQueryBuilder::Build() const -> std::string {
  Expects(table_.has_value());

  const auto *const columns = std::visit(
      [](const auto &variant) -> const std::vector<Column> * {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, std::vector<Column>>) {
          return &variant;
        }

        if constexpr (std::is_same_v<T, AllColumnsType>) {
          return nullptr;
        }

        Expects(false);
      },
      columns_);

  return query_builder_->BuildSelectQuery(
      *table_, columns, where_clause_.value_or(std::string{}));
}
}  // namespace stonks::sqldb