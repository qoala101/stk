#ifndef VH_SQLDB_QB_DROP_H_
#define VH_SQLDB_QB_DROP_H_

#include <string>

#include "cpp_meta_template_constructor.h"
#include "sqldb_concepts.h"  // IWYU pragma: keep
#include "sqldb_types.h"

namespace vh::sqldb::qb {
/**
 * @brief Builds drop table query.
 */
class Drop {
 public:
  /**
   * @tparam Table Table to drop.
   */
  template <TableDefinition Table>
  explicit Drop(cpp::meta::TemplateConstructor<Table> /*unused*/)
      : Drop{Table::GetName()} {}

  /**
   * @brief Builds the query.
   */
  auto Build [[nodiscard]] () const -> Query;

 private:
  explicit Drop(std::string table_name);

  Query table_name_{};
};
}  // namespace vh::sqldb::qb

#endif  // VH_SQLDB_QB_DROP_H_
