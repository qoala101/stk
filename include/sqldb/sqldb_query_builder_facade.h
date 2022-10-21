#ifndef STONKS_SQLDB_SQLDB_QUERY_BUILDER_FACADE_H_
#define STONKS_SQLDB_SQLDB_QUERY_BUILDER_FACADE_H_

#include "di_factory.h"
#include "sqldb_i_query_builder.h"
#include "sqldb_qbf_delete_query_builder.h"
#include "sqldb_qbf_insert_query_builder.h"
#include "sqldb_qbf_select_query_builder.h"
#include "sqldb_qbf_update_query_builder.h"
#include "sqldb_traits.h"
#include "sqldb_types.h"

namespace stonks::sqldb {
/**
 * @brief Start point for query builders which provide fluent query-building
 * API.
 */
class QueryBuilderFacade {
 public:
  explicit QueryBuilderFacade(di::Factory<IQueryBuilder> query_builder_factory);

  /**
   * @brief Start building select statement.
   */
  [[nodiscard]] auto Select() const -> qbf::SelectQueryBuilder;

  /**
   * @brief Start building insert statement.
   */
  [[nodiscard]] auto Insert() const -> qbf::InsertQueryBuilder;

  /**
   * @brief Start building update statement.
   */
  [[nodiscard]] auto Update() const -> qbf::UpdateQueryBuilder;

  /**
   * @brief Start building delete statement.
   */
  [[nodiscard]] auto Delete() const -> qbf::DeleteQueryBuilder;

 private:
  di::Factory<IQueryBuilder> query_builder_factory_;
};

class QueryBuilderFacadeT {
 public:
  template <typename... Columns>
  [[nodiscard]] auto Select() {
    return qbf::SelectQueryBuilderTemplate{
        static_cast<std::tuple<Columns...> *>(nullptr)};
  }

  [[nodiscard]] auto SelectAll() {
    return qbf::SelectQueryBuilderTemplate{static_cast<qbf::All *>(nullptr)};
  }

  [[nodiscard]] auto SelectOne() {
    return qbf::SelectQueryBuilderTemplate{static_cast<qbf::One *>(nullptr)};
  }

  [[nodiscard]] auto Insert() { return qbf::InsertQueryBuilderTemplate{}; }

  [[nodiscard]] auto Delete() { return qbf::DeleteQueryBuilderTemplate{}; }

  template <typename Table>
  [[nodiscard]] auto Update() {
    return qbf::UpdateQueryBuilderTemplate{static_cast<Table *>(nullptr)};
  }
};

namespace qbf {
template <typename ColumnT>
struct Column {};

struct Param {
  Param() : text_{"?"} {}

  explicit Param(SelectQueryBuilderTemplate builder)
      : text_{fmt::format("({})", builder.Build().first.value)} {}

  std::string text_{};
};

template <typename LeftColumn, typename RightColumn>
[[nodiscard]] auto operator==(const Column<LeftColumn> &left,
                              const Column<RightColumn> &right) {
  return fmt::format("{} = {}", ColumnTraits<LeftColumn>::GetFullName(),
                     ColumnTraits<RightColumn>::GetFullName());
}

template <typename LeftColumn>
[[nodiscard]] auto operator==(const Column<LeftColumn> &left,
                              const Param &right) {
  return fmt::format("{} = {}", ColumnTraits<LeftColumn>::GetFullName(),
                     right.text_);
}

template <typename LeftColumn>
[[nodiscard]] auto operator>=(const Column<LeftColumn> &left,
                              const Param &right) {
  return fmt::format("{} >= {}", ColumnTraits<LeftColumn>::GetFullName(),
                     right.text_);
}

template <typename LeftColumn>
[[nodiscard]] auto operator<(const Column<LeftColumn> &left,
                             const Param &right) {
  return fmt::format("{} < {}", ColumnTraits<LeftColumn>::GetFullName(),
                     right.text_);
}

[[nodiscard]] inline auto Exists(SelectQueryBuilderTemplate builder) {
  return fmt::format("EXISTS ({})", builder.Build().first.value);
};

[[nodiscard]] inline auto On(std::string_view on_clause) {
  return fmt::format("ON ({})", on_clause);
};
}  // namespace qbf
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_QUERY_BUILDER_FACADE_H_
