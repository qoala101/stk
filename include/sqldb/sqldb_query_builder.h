#ifndef STONKS_SQLDB_SQLDB_QUERY_BUILDER_H_
#define STONKS_SQLDB_SQLDB_QUERY_BUILDER_H_

#include "cpp_meta_template_constructor.h"
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
auto CreateTable [[nodiscard]] () {
  return qb::Create{cpp::meta::TemplateConstructor<Table>{}};
}

/**
 * @brief Builds query which drops the table.
 */
template <TableDefinition Table>
auto DropTable [[nodiscard]] () {
  return qb::Drop{cpp::meta::TemplateConstructor<Table>{}};
}

/**
 * @brief Builds query which selects specified columns.
 */
template <ColumnDefinition... Columns>
auto Select [[nodiscard]] (cpp::Opt<qb::Distinct> distinct = {}) {
  return qb::Select{cpp::meta::TemplateConstructor<Columns...>{},
                    distinct.has_value()};
}

/**
 * @brief Builds query which selects all columns from table.
 */
auto SelectAll [[nodiscard]] (cpp::Opt<qb::Distinct> distinct = {})
-> qb::Select;

/**
 * @brief Builds special query which is used to check existence of the
 * condition.
 */
auto SelectOne [[nodiscard]] () -> qb::Select;

/**
 * @brief Builds query which inserts values to the specified columns.
 */
auto Insert [[nodiscard]] () -> qb::Insert;

/**
 * @brief Builds query which inserts values to each of the table columns.
 */
auto InsertAll [[nodiscard]] () -> qb::Insert;

/**
 * @brief Builds query which updates the values in specified columns.
 */
template <TableDefinition Table>
auto UpdateTable [[nodiscard]] () {
  return qb::Update{cpp::meta::TemplateConstructor<Table>{}};
}

/**
 * @brief Builds query which deletes rows from the table.
 */
template <TableDefinition Table>
auto DeleteFromTable [[nodiscard]] () {
  return qb::Delete{cpp::meta::TemplateConstructor<Table>{}};
}
}  // namespace stonks::sqldb::query_builder

#endif  // STONKS_SQLDB_SQLDB_QUERY_BUILDER_H_
