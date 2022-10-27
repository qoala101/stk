#include "sqldb_p_common.h"

#include <range/v3/algorithm/equal.hpp>
#include <range/v3/view/transform.hpp>

namespace stonks::sqldb::p {
auto AreParamsValid(const std::vector<Value> &values,
                    const std::vector<Param> &params) -> bool {
  return (values.size() == params.size()) &&
         ranges::equal(
             values | ranges::views::transform(
                          [](const Value &value) { return value.GetType(); }),
             params);
}
}  // namespace stonks::sqldb::p