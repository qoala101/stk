#include "sqldb_qb_common.h"

#include <gsl/assert>
#include <memory>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <variant>
#include <vector>

#include "cpp_not_null.h"
#include "not_null.hpp"
#include "sqldb_qb_select.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {

  Param::Param(const Select &builder)
      : text_{fmt::format("({})", builder.Build().query.value)} {}


auto Exists(const Select &builder) -> WhereQuery{
  return {fmt::format("EXISTS ({})", builder.Build().query.value)};
};
}  // namespace stonks::sqldb::qb
