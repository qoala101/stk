#ifndef STONKS_SQLDB_SQLDB_QUERY_BUILDER_H_
#define STONKS_SQLDB_SQLDB_QUERY_BUILDER_H_

#include "di_factory.h"
#include "sqldb_qb_create.h"
#include "sqldb_qb_delete.h"
#include "sqldb_qb_drop.h"
#include "sqldb_qb_insert.h"
#include "sqldb_qb_select.h"
#include "sqldb_qb_update.h"
#include "sqldb_traits.h"
#include "sqldb_types.h"

namespace stonks::sqldb::query_builder {
template <typename Table>
[[nodiscard]] auto CreateTable() {
  return qb::Create{static_cast<Table *>(nullptr)};
}

template <typename Table>
[[nodiscard]] auto DropTable() {
  return qb::Drop{static_cast<Table *>(nullptr)};
}

template <typename... Columns>
[[nodiscard]] auto Select() {
  return qb::Select{static_cast<std::tuple<Columns...> *>(nullptr)};
}

[[nodiscard]] auto SelectAll() -> qb::Select;

[[nodiscard]] auto SelectOne() -> qb::Select;

[[nodiscard]] auto Insert() -> qb::Insert;

[[nodiscard]] auto InsertAll() -> qb::Insert;

template <typename Table>
[[nodiscard]] auto UpdateTable() {
  return qb::Update{static_cast<Table *>(nullptr)};
}

template <typename Table>
[[nodiscard]] auto DeleteFromTable() {
  return qb::Delete{static_cast<Table *>(nullptr)};
}
}  // namespace stonks::sqldb::query_builder

#endif  // STONKS_SQLDB_SQLDB_QUERY_BUILDER_H_
