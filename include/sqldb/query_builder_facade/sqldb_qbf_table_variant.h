#ifndef STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_TABLE_VARIANT_H_
#define STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_TABLE_VARIANT_H_

#include <variant>

#include "cpp_copy_const.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qbf {
namespace detail {
using TableVariantType = std::variant<std::monostate, Table, TableDefinition>;
}  // namespace detail

/**
 * @brief Stores table value in query builders.
 */
class TableVariant : public detail::TableVariantType {
 public:
  using detail::TableVariantType::variant;

  /**
   * @brief Gives table from variant.
   * @remark Should only be called if variant has table.
   */
  [[nodiscard]] auto GetTable() const -> const Table &;

  /**
   * @copydoc GetTable
   */
  [[nodiscard]] auto GetTable() -> Table &;

  /**
   * @brief Whether variant has table.
   */
  [[nodiscard]] auto HasTable() const -> bool;

 private:
  template <cpp::DecaysTo<TableVariant> This>
  [[nodiscard]] static auto GetTableImpl(This &t)
      -> cpp::CopyConst<This, Table &>;
};

}  // namespace stonks::sqldb::qbf

#endif  // STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_TABLE_VARIANT_H_
