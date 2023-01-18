#ifndef STONKS_SQLDB_PARAMETRIZED_SQLDB_P_TYPES_H_
#define STONKS_SQLDB_PARAMETRIZED_SQLDB_P_TYPES_H_

#include <concepts>
#include <vector>

#include "cpp_typed_struct.h"
#include "sqldb_types.h"

namespace stonks::sqldb::prm {
/**
 * @brief Query parameter to be substituted with value
 * when executing the statement.
 */
struct QueryParam : public cpp::TypedStruct<DataTypeVariant> {};

/**
 * @brief List of params with convenient API.
 */
struct QueryParams : public cpp::TypedStruct<std::vector<QueryParam>> {
  /**
   * @brief Appends params to the list.
   */
  void operator+=(const QueryParams &other);
};

/**
 * @brief Query with known parameter types.
 */
template <std::derived_from<Query> Query>
struct Parametrized : public Query {
  QueryParams params{};
};
}  // namespace stonks::sqldb::p

#endif  // STONKS_SQLDB_PARAMETRIZED_SQLDB_P_TYPES_H_
