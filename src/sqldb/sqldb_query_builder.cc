#include "sqldb_query_builder.h"

#include "cpp_optional.h"
#include "sqldb_qb_insert.h"
#include "sqldb_qb_select.h"
#include "sqldb_qb_types.h"

namespace stonks::sqldb::query_builder {
auto SelectAll(cpp::Opt<qb::Distinct> distinct) -> qb::Select {
  return qb::Select{qb::All{}, distinct.has_value()};
}

auto SelectOne() -> qb::Select { return qb::Select{qb::One{}}; }

auto Insert() -> qb::Insert { return {}; }

auto InsertAll() -> qb::Insert { return qb::Insert{qb::All{}}; }
}  // namespace stonks::sqldb::query_builder