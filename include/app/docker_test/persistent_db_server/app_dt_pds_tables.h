#ifndef STONKS_APP_DOCKER_TEST_PERSISTENT_DB_SERVER_APP_DT_PDS_TABLES_H_
#define STONKS_APP_DOCKER_TEST_PERSISTENT_DB_SERVER_APP_DT_PDS_TABLES_H_

#include "sqldb_types.h"

namespace stonks::app::dt::pds::tables {
[[nodiscard]] auto SymbolPriceRecord() -> const sqldb::TableDefinition &;
}  // namespace stonks::app::dt::pds::tables

#endif  // STONKS_APP_DOCKER_TEST_PERSISTENT_DB_SERVER_APP_DT_PDS_TABLES_H_
