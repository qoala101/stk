#ifndef STONKS_SQLDB_SQLDB_QUERY_BUILDER_H_
#define STONKS_SQLDB_SQLDB_QUERY_BUILDER_H_

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
template <typename Table>
[[nodiscard]] auto CreateTable() {
  return qb::Create{static_cast<Table *>(nullptr)};
}

/**
 * @brief Builds query which drops the table.
 */
template <typename Table>
[[nodiscard]] auto DropTable() {
  return qb::Drop{static_cast<Table *>(nullptr)};
}

/**
 * @brief Builds query which selects specified columns.
 */
template <typename... Columns>
[[nodiscard]] auto Select() {
  return qb::Select{static_cast<std::tuple<Columns...> *>(nullptr)};
}

/**
 * @brief Builds query which selects all columns from table.
 */
[[nodiscard]] auto SelectAll() -> qb::Select;

/**
 * @brief Builds query which selects 1.
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
 * @brief Builds query which updates he values in specified columns.
 */
template <typename Table>
[[nodiscard]] auto UpdateTable() {
  return qb::Update{static_cast<Table *>(nullptr)};
}

/**
 * @brief Builds query which deletes rows from the table.
 */
template <typename Table>
[[nodiscard]] auto DeleteFromTable() {
  return qb::Delete{static_cast<Table *>(nullptr)};
}
}  // namespace stonks::sqldb::query_builder

#endif  // STONKS_SQLDB_SQLDB_QUERY_BUILDER_H_
