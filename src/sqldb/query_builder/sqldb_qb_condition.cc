#include "sqldb_qb_condition.h"

#include <fmt/format.h>

#include <gsl/assert>

namespace stonks::sqldb::qb {
Condition::Condition(std::string query)
    : query_{[&query]() {
        Expects(!query.empty());
        return std::move(query);
      }()} {
  Ensures(!query_.empty());
}

auto Condition::operator&&(const Condition &condition) -> Condition & {
  query_ += fmt::format(" AND ({})", condition.query_);
  return *this;
}

auto Condition::operator||(const Condition &condition) -> Condition & {
  query_ += fmt::format(" OR ({})", condition.query_);
  return *this;
}

auto Condition::GetQuery() const -> const std::string & { return query_; }
}  // namespace stonks::sqldb::qb