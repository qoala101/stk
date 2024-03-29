/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_SQLDB_QB_QUERY_WRAPPER_H_
#define VH_SQLDB_QB_QUERY_WRAPPER_H_

#include "cpp_this.h"
#include "sqldb_prm_types.h"
#include "sqldb_types.h"

namespace vh::sqldb::qb {
/**
 * @brief Wraps the query.
 */
class QueryWrapper {
 public:
  /**
   * @brief Gives the value of the query.
   */
  auto GetQuery [[nodiscard]] () const -> const prm::Parametrized<Query> &;
  auto GetQuery [[nodiscard]] () -> prm::Parametrized<Query> &;

 protected:
  explicit QueryWrapper(prm::Parametrized<Query> query);

 private:
  static auto GetQueryImpl [[nodiscard]] (cpp::This<QueryWrapper> auto &t)
  -> auto &;

  prm::Parametrized<Query> query_{};
};
}  // namespace vh::sqldb::qb

#endif  // VH_SQLDB_QB_QUERY_WRAPPER_H_
