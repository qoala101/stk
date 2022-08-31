#ifndef STONKS_NOSQLDB_NOSQLDB_I_DB_H_
#define STONKS_NOSQLDB_NOSQLDB_I_DB_H_

#include "nosqldb_i_items_interface.h"
#include "nosqldb_i_tables_interface.h"

namespace stonks::nosqldb {
/**
 * @brief Generic NoSQL DB interface.
 */
class IDb : public ITablesInterface, public IItemsInterface {
 protected:
  explicit IDb() = default;
};
}  // namespace stonks::nosqldb

#endif  // STONKS_NOSQLDB_NOSQLDB_I_DB_H_
