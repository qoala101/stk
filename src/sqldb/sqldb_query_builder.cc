#include "sqldb_query_builder.h"

#include <utility>

#include "cpp_expose_private_constructors.h"
#include "sqldb_qb_delete.h"
#include "sqldb_qb_insert.h"
#include "sqldb_qb_select.h"

namespace stonks::sqldb::query_builder {
auto SelectAll() -> qb::Select {
  return qb::Select{static_cast<qb::All *>(nullptr)};
}

auto SelectOne() -> qb::Select {
  return qb::Select{static_cast<qb::One *>(nullptr)};
}

auto Insert() -> qb::Insert { return {}; }

auto InsertAll() -> qb::Insert {
  return qb::Insert{static_cast<qb::All *>(nullptr)};
}
}  // namespace stonks::sqldb::query_builder