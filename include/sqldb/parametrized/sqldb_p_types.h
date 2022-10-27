#ifndef STONKS_SQLDB_PARAMETRIZED_SQLDB_P_TYPES_H_
#define STONKS_SQLDB_PARAMETRIZED_SQLDB_P_TYPES_H_

#include "cpp_typed_struct.h"
#include "sqldb_types.h"

namespace stonks::sqldb::p {
/**
 * @brief Query parameter to be substituted with value
 * when executing the statement.
 */
struct Param : public cpp::TypedStruct<DataTypeVariant> {};

/**
 * @brief List of params with convenient API.
 */
struct Params : public cpp::TypedStruct<std::vector<Param>> {
  /**
   * @brief Appends params to the list.
   */
  void Append(const Params &params);
};

/**
 * @brief Query with known parameter types.
 */
template <typename Query>
struct Parametrized : public Query {
  Params params{};
};
}  // namespace stonks::sqldb::p

#endif  // STONKS_SQLDB_PARAMETRIZED_SQLDB_P_TYPES_H_
