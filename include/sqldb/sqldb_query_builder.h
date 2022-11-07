#ifndef STONKS_SQLDB_SQLDB_QUERY_BUILDER_H_
#define STONKS_SQLDB_SQLDB_QUERY_BUILDER_H_

#include "cpp_template_constructor.h"
#include "sqldb_concepts.h"  // IWYU pragma: keep
#include "sqldb_qb_create.h"
#include "sqldb_qb_delete.h"
#include "sqldb_qb_drop.h"
#include "sqldb_qb_insert.h"
#include "sqldb_qb_select.h"
#include "sqldb_qb_update.h"

namespace stonks::sqldb::query_builder {
/**
 * @brief Builds query which creates the table from its definition.
 */
template <TableDefinition Table>
[[nodiscard]] auto CreateTable() {
  return qb::Create{cpp::TemplateConstructor<Table>{}};
}

/**
 * @brief Builds query which drops the table.
 */
template <TableDefinition Table>
[[nodiscard]] auto DropTable() {
  return qb::Drop{cpp::TemplateConstructor<Table>{}};
}

/**
 * @brief Builds query which selects specified columns.
 */
template <ColumnDefinition... Columns>
[[nodiscard]] auto Select() {
  return qb::Select{cpp::TemplateConstructor<Columns...>{}};
}

/**
 * @brief Builds query which selects all columns from table.
 */
[[nodiscard]] auto SelectAll() -> qb::Select;

/**
 * @brief Builds special query which is used to check existence of the
 * condition.
 */
[[nodiscard]] auto SelectOne() -> qb::Select;

/**
 * @brief Builds query which inserts values to the specified columns.
 */
[[nodiscard]] auto Insert() -> qb::Insert;

/**
 * @brief Builds query which inserts values to each of the table columns.
 */
[[nodiscard]] auto InsertAll() -> qb::Insert;

/**
 * @brief Builds query which updates the values in specified columns.
 */
template <TableDefinition Table>
[[nodiscard]] auto UpdateTable() {
  return qb::Update{cpp::TemplateConstructor<Table>{}};
}

/**
 * @brief Builds query which deletes rows from the table.
 */
template <TableDefinition Table>
[[nodiscard]] auto DeleteFromTable() {
  return qb::Delete{cpp::TemplateConstructor<Table>{}};
}
}  // namespace stonks::sqldb::query_builder

#endif  // STONKS_SQLDB_SQLDB_QUERY_BUILDER_H_
