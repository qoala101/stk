#ifndef STONKS_APP_DOCKER_TEST_PERSISTENT_DB_SERVER_APP_DT_PDS_APP_H_
#define STONKS_APP_DOCKER_TEST_PERSISTENT_DB_SERVER_APP_DT_PDS_APP_H_

#include "app_dt_pds_db.h"
#include "app_dt_types.h"

namespace stonks::app::dt::pds {
class App {
 public:
  explicit App(Db db);

  void RecordSymbolPrice(const SymbolPriceRecord &record);

 private:
  Db db_;
};
}  // namespace stonks::app::dt::pds

#endif  // STONKS_APP_DOCKER_TEST_PERSISTENT_DB_SERVER_APP_DT_PDS_APP_H_
