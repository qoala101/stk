#ifndef STONKS_SQLDB_PARAMETRIZED_SQLDB_P_TYPES_H_
#define STONKS_SQLDB_PARAMETRIZED_SQLDB_P_TYPES_H_

#include "sqldb_types.h"

namespace stonks::sqldb::p {
/**
 * @brief Query parameter to be substituted with value
 * when executing the statement.
 */
struct Param : public cpp::TypedStruct<DataTypeVariant> {};

/**
 * @brief Query with known parameter types.
 */
template <typename Query>
struct Parametrized : public Query {
  std::vector<Param> params{};
};
}  // namespace stonks::sqldb::p

#endif  // STONKS_SQLDB_PARAMETRIZED_SQLDB_P_TYPES_H_
