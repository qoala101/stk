#ifndef STONKS_KVDB_KVDB_I_TABLES_INTERFACE_H_
#define STONKS_KVDB_KVDB_I_TABLES_INTERFACE_H_

#include "kvdb_types.h"

namespace stonks::kvdb {
/**
 * @brief Generic Key-Value DB interface to manipulate the tables.
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
}  // namespace stonks::kvdb

#endif  // STONKS_KVDB_KVDB_I_TABLES_INTERFACE_H_
