#ifndef STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_LIMIT_VARIANT_H_
#define STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_LIMIT_VARIANT_H_

#include <string>
#include <variant>

namespace stonks::sqldb::qbf {
/**
 * @brief Variant of limit specification.
 */
struct LimitedType {};

namespace detail {
using LimitVariantType = std::variant<std::monostate, int, LimitedType>;
}  // namespace detail

/**
 * @brief Stores limit value in query builders.
 */
class LimitVariant : public detail::LimitVariantType {
 public:
  using detail::LimitVariantType::variant;

  /**
   * @brief Gives limit clause which can be used in queries.
   */
  [[nodiscard]] auto GetLimitClause() const -> std::string;
};
}  // namespace stonks::sqldb::qbf

#endif  // STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_LIMIT_VARIANT_H_
