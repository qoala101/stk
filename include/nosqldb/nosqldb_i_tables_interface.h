#ifndef STONKS_NOSQLDB_NOSQLDB_I_TABLES_INTERFACE_H_
#define STONKS_NOSQLDB_NOSQLDB_I_TABLES_INTERFACE_H_

#include "cpp_optional.h"
#include "nosqldb_types.h"

namespace stonks::nosqldb {
/**
 * @brief Generic NoSQL DB interface to manipulate the tables.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class ITablesInterface {
 public:
  virtual ~ITablesInterface() = default;

  /**
   * @brief Creates a table with the specified name if it doesn't exist.
   */
  virtual void CreateTableIfNotExists(const Table &table) = 0;

  /**
   * @brief Deletes the table from DB.
   */
  virtual void DropTableIfExists(const Table &table) = 0;
};
}  // namespace stonks::nosqldb

#endif  // STONKS_NOSQLDB_NOSQLDB_I_TABLES_INTERFACE_H_
