#include "sqldb_prm_common.h"

#include <range/v3/algorithm/equal.hpp>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/functional/comparisons.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/functional/invoke.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <variant>

#include "cpp_typed_struct.h"
#include "sqldb_types.h"

namespace vh::sqldb::prm {
auto AreParamsValid(const std::vector<Value> &values,
                    const std::vector<QueryParam> &params) -> bool {
  return (values.size() == params.size()) &&
         ranges::equal(values | ranges::views::transform([](const auto &value) {
                         return value.GetType();
                       }),
                       params);
}
}  // namespace vh::sqldb::prm