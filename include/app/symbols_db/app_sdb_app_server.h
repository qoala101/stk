#ifndef STONKS_APP_SYMBOLS_DB_APP_SDB_APP_SERVER_H_
#define STONKS_APP_SYMBOLS_DB_APP_SDB_APP_SERVER_H_

#include "app_sdb_app.h"
#include "network_rest_server.h"
#include "network_rest_server_builder.h"

namespace stonks::app::sdb {
class AppServer {
 public:
  AppServer(App app, network::RestServerBuilder rest_server_builder);

 private:
  network::RestServer rest_server_;
};
}  // namespace stonks::app::sdb

#endif  // STONKS_APP_SYMBOLS_DB_APP_SDB_APP_SERVER_H_
