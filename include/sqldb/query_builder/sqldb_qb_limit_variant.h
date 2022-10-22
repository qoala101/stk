#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QBF_LIMIT_VARIANT_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QBF_LIMIT_VARIANT_H_

#include <string>
#include <variant>

#include "cpp_variant_struct.h"

namespace stonks::sqldb::qb {
/**
 * @brief Variant of limit specification.
 */
struct LimitedType {};

/**
 * @brief Stores limit value in query builders.
 */
class LimitVariant
    : public cpp::VariantStruct<std::monostate, int, LimitedType> {
 public:
  /**
   * @brief Gives limit clause which can be used in queries.
   */
  [[nodiscard]] auto GetLimitClause() const -> std::string;
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QBF_LIMIT_VARIANT_H_
