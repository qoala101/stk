#ifndef STONKS_APP_DOCKER_TEST_PERSISTENT_DB_SERVER_APP_DT_PDS_TABLES_H_
#define STONKS_APP_DOCKER_TEST_PERSISTENT_DB_SERVER_APP_DT_PDS_TABLES_H_

#include "sqldb_types.h"

namespace stonks::app::dt::pds::tables {
struct SymbolPriceRecord {
  static const auto table = {
      .table = {"SymbolPriceRecord"},
      .columns = {
          {.column = {"id"},
           .data_type = sqldb::DataType::kInt64,
           .primary_key = true,
           .auto_increment = true,
           .unique = true},
          {.column = {"time"}, .data_type = sqldb::DataType::kInt64},
          {.column = {"symbol"}, .data_type = sqldb::DataType::kString},
          {.column = {"price"}, .data_type = sqldb::DataType::kDouble}}};
};
}  // namespace stonks::app::dt::pds::tables

#endif  // STONKS_APP_DOCKER_TEST_PERSISTENT_DB_SERVER_APP_DT_PDS_TABLES_H_
