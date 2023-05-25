/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "sqldb_query_builder.h"

#include "cpp_optional.h"
#include "sqldb_qb_insert.h"
#include "sqldb_qb_select.h"
#include "sqldb_qb_types.h"

namespace vh::sqldb::query_builder {
auto SelectAll(cpp::Opt<qb::Distinct> distinct) -> qb::Select {
  return qb::Select{qb::All{}, distinct.has_value()};
}

auto SelectOne() -> qb::Select { return qb::Select{qb::One{}}; }

auto Insert() -> qb::Insert { return {}; }

auto InsertAll() -> qb::Insert { return qb::Insert{qb::All{}}; }
}  // namespace vh::sqldb::query_builder