#ifndef STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_COLUMNS_VARIANT_H_
#define STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_COLUMNS_VARIANT_H_

#include <variant>
#include <vector>

#include "cpp_this.h"  // IWYU pragma: keep
#include "cpp_variant_struct.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qbf {
/**
 * @brief Variant of columns specification.
 */
struct AllColumnsType {};

/**
 * @brief Stores columns value in query builders.
 */
class ColumnsVariant
    : public cpp::VariantStruct<std::monostate, std::vector<Column>,
                                AllColumnsType> {
 public:
  /**
   * @brief Gives columns from variant. Null if all columns variant is used.
   * @remark Should only be called if variant has columns.
   */
  [[nodiscard]] auto GetColumns() const -> const std::vector<Column> *;
  [[nodiscard]] auto GetColumns() -> std::vector<Column> *;

  /**
   * @brief Whether variant has columns.
   */
  [[nodiscard]] auto HasColumns() const -> bool;

 private:
  [[nodiscard]] static auto GetColumnsImpl(cpp::This<ColumnsVariant> auto &t);
};
}  // namespace stonks::sqldb::qbf

#endif  // STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_COLUMNS_VARIANT_H_
