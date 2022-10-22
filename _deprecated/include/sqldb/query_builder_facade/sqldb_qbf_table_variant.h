#ifndef STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_TABLE_VARIANT_H_
#define STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_TABLE_VARIANT_H_

#include <variant>

#include "cpp_this.h"  // IWYU pragma: keep
#include "cpp_variant_struct.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qbf {
/**
 * @brief Stores table value in query builders.
 */
class TableVariant
    : public cpp::VariantStruct<std::monostate, Table, TableDefinition> {
 public:
  /**
   * @brief Gives table from variant.
   * @remark Should only be called if variant has table.
   */
  [[nodiscard]] auto GetTable() const -> const Table &;
  [[nodiscard]] auto GetTable() -> Table &;

  /**
   * @brief Whether variant has table.
   */
  [[nodiscard]] auto HasTable() const -> bool;

 private:
  [[nodiscard]] static auto GetTableImpl(cpp::This<TableVariant> auto &t)
      -> auto &;
};

}  // namespace stonks::sqldb::qbf

#endif  // STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_TABLE_VARIANT_H_
