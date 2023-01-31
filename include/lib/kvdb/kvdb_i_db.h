#ifndef VH_KVDB_I_DB_H_
#define VH_KVDB_I_DB_H_

#include "kvdb_i_items_interface.h"
#include "kvdb_i_tables_interface.h"

namespace vh::kvdb {
/**
 * @brief Generic Key-Value DB interface.
 */
class IDb : public ITablesInterface, public IItemsInterface {};
}  // namespace vh::kvdb

#endif  // VH_KVDB_I_DB_H_
