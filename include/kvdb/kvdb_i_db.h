#ifndef STONKS_KVDB_KVDB_I_DB_H_
#define STONKS_KVDB_KVDB_I_DB_H_

#include "kvdb_i_items_interface.h"
#include "kvdb_i_tables_interface.h"

namespace stonks::kvdb {
/**
 * @brief Generic Key-Value DB interface.
 */
class IDb : public ITablesInterface, public IItemsInterface {};
}  // namespace stonks::kvdb

#endif  // STONKS_KVDB_KVDB_I_DB_H_
