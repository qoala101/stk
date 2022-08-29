#ifndef STONKS_NOSQLDB_NOSQLDB_TYPES_H_
#define STONKS_NOSQLDB_NOSQLDB_TYPES_H_

#include <string>

/**
 * @file Common NoSQL DB related types.
 */

namespace stonks::nosqldb {
using Table = std::string;
using Key = std::string;
using Value = std::string;
}  // namespace stonks::nosqldb

#endif  // STONKS_NOSQLDB_NOSQLDB_TYPES_H_
