#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_INSERT_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_INSERT_H_

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
#include "sqldb_table_traits.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
struct All;

class Insert {
 public:
  Insert() = default;

  explicit Insert(All* /*unused*/) : insert_all_{true} {}

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
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_INSERT_H_
