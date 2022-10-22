#ifndef STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_INSERT_QUERY_BUILDER_H_
#define STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_INSERT_QUERY_BUILDER_H_

#include <fmt/format.h>

#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/view/repeat.hpp>
#include <range/v3/view/repeat_n.hpp>
#include <range/v3/view/single.hpp>
#include <vector>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "cpp_views.h"
#include "sqldb_qbf_columns_variant.h"
#include "sqldb_qbf_table_variant.h"
#include "sqldb_traits.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qbf {
struct All;

class InsertQueryBuilderTemplate {
 public:
  InsertQueryBuilderTemplate() = default;

  explicit InsertQueryBuilderTemplate(All* /*unused*/) : insert_all_{true} {}

  template <typename Column>
  [[nodiscard]] auto Value(std::string TEMP) -> auto& {
    if (!column_names_.empty()) {
      column_names_ += ", ";
    }

    column_names_ += ColumnTraits<Column>::GetName();

    if (!values_.empty()) {
      values_ += ", ";
    }

    values_ += TEMP;

    return *this;
  }

  template <typename Table>
  [[nodiscard]] auto Into() -> auto& {
    Expects(table_name_.empty());
    table_name_ = TableTraits<Table>::GetName();

    if (insert_all_) {
      column_names_ = ColumnsTraits<typename Table::Columns>::GetNames();
      values_ = ranges::accumulate(
          ranges::views::repeat_n(
              fmt::format(", {}", "?"),
              ColumnsTraits<typename Table::Columns>::GetSize() - 1),
          std::string{"?"});
    }

    Ensures(!table_name_.empty());
    return *this;
  }

  [[nodiscard]] auto Build() const -> Query;

 private:
  bool insert_all_{};
  std::string column_names_{};
  std::string values_{};
  std::string table_name_{};
};
}  // namespace stonks::sqldb::qbf

#endif  // STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_INSERT_QUERY_BUILDER_H_
