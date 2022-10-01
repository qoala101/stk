#ifndef STONKS_APP_DOCKER_TEST_PERSISTENT_DB_SERVER_APP_DT_PDS_DB_H_
#define STONKS_APP_DOCKER_TEST_PERSISTENT_DB_SERVER_APP_DT_PDS_DB_H_

#include "app_dt_types.h"
#include "cpp_not_null.h"
#include "sqldb_i_db.h"
#include "sqldb_i_query_builder.h"
#include "sqldb_i_update_statement.h"

namespace stonks::app::dt::pds {
class Db {
 public:
  Db(sqldb::IDb &db, const cpp::NnSp<sqldb::IQueryBuilder> &query_builder);

  void InsertSymbolPriceRecord(const SymbolPriceRecord &record);

 private:
  cpp::NnUp<sqldb::IUpdateStatement> insert_symbol_price_record_statement_;
};
}  // namespace stonks::app::dt::pds

#endif  // STONKS_APP_DOCKER_TEST_PERSISTENT_DB_SERVER_APP_DT_PDS_DB_H_
