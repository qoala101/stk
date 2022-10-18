#include "sqldb_qbf_table_variant.h"

#include <gsl/assert>
#include <variant>

#include "cpp_copy_const.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qbf {
template <cpp::This<TableVariant> This>
auto TableVariant::GetTableImpl(This &t) -> auto & {
  Expects(t.HasTable());
  return std::visit(
      [](auto &v) -> cpp::CopyConst<This, Table> & {
        using V = decltype(v);

        if constexpr (cpp::DecaysTo<V, Table>) {
          return v;
        }

        if constexpr (cpp::DecaysTo<V, TableDefinition>) {
          return v.table;
        }

        Expects(false);
      },
      t.value);
}

auto TableVariant::GetTable() const -> const Table & {
  return GetTableImpl(*this);
}

auto TableVariant::GetTable() -> Table & { return GetTableImpl(*this); }

auto TableVariant::HasTable() const -> bool {
  return !std::holds_alternative<std::monostate>(value);
}
}  // namespace stonks::sqldb::qbf
