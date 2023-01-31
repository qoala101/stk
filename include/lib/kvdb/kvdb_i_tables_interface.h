#ifndef VH_KVDB_I_TABLES_INTERFACE_H_
#define VH_KVDB_I_TABLES_INTERFACE_H_

#include <cppcoro/task.hpp>

#include "kvdb_types.h"

namespace vh::kvdb {
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
  virtual auto CreateTableIfNotExists(const Table &table)
      -> cppcoro::task<> = 0;

  /**
   * @brief Deletes the table from DB.
   */
  virtual auto DropTableIfExists(const Table &table) -> cppcoro::task<> = 0;
};
}  // namespace vh::kvdb

#endif  // VH_KVDB_I_TABLES_INTERFACE_H_
