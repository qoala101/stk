#include "sqldb_qbf_columns_variant.h"

#include <gsl/assert>
#include <variant>

#include "sqldb_types.h"

namespace stonks::sqldb::qbf {
template <cpp::DecaysTo<ColumnsVariant> This>
auto ColumnsVariant::GetColumnsImpl(This &t)
    -> cpp::CopyConst<This, std::vector<Column>> * {
  Expects(t.HasColumns());
  return std::visit(
      [](auto &v) -> cpp::CopyConst<This, std::vector<Column>> * {
        using V = decltype(v);

        if constexpr (cpp::DecaysTo<V, std::vector<Column>>) {
          return &v;
        }

        if constexpr (cpp::DecaysTo<V, AllColumnsType>) {
          return nullptr;
        }

        Expects(false);
      },
      t.value);
}

auto ColumnsVariant::GetColumns() const -> const std::vector<Column> * {
  return GetColumnsImpl(*this);
}

auto ColumnsVariant::GetColumns() -> std::vector<Column> * {
  return GetColumnsImpl(*this);
}

auto ColumnsVariant::HasColumns() const -> bool {
  return !std::holds_alternative<std::monostate>(value);
}
}  // namespace stonks::sqldb::qbf