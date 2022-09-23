#include "sqldb_qbf_limit_variant.h"

#include <fmt/core.h>

#include "cpp_concepts.h"  // IWYU pragma: keep

namespace stonks::sqldb::qbf {
auto LimitVariant::GetLimitClause() const -> std::string {
  return std::visit(
      [](const auto &v) -> std::string {
        using V = decltype(v);

        if constexpr (cpp::DecaysTo<V, int>) {
          return fmt::format(" LIMIT {}", v);
        }

        if constexpr (cpp::DecaysTo<V, LimitedType>) {
          return " LIMIT ?";
        }

        return {};
      },
      static_cast<const detail::LimitVariantType &>(*this));
}
}  // namespace stonks::sqldb::qbf