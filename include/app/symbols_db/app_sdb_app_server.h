#ifndef STONKS_APP_SYMBOLS_DB_APP_SDB_APP_SERVER_H_
#define STONKS_APP_SYMBOLS_DB_APP_SDB_APP_SERVER_H_

#include "app_sdb_app.h"
#include "cpp_not_null.h"
#include "network_i_rest_request_receiver.h"
#include "network_rest_server.h"
#include "network_types.h"

namespace stonks::app::sdb {
class AppServer {
 public:
  AppServer(App app, network::Uri base_uri,
            cpp::NnUp<network::IRestRequestReceiver> request_receiver);

 private:
  network::RestServer rest_server_;
};
}  // namespace stonks::app::sdb

#endif  // STONKS_APP_SYMBOLS_DB_APP_SDB_APP_SERVER_H_
