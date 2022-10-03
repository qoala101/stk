#ifndef STONKS_APP_DOCKER_TEST_PERSISTENT_DB_SERVER_APP_DT_PDS_APP_SERVER_H_
#define STONKS_APP_DOCKER_TEST_PERSISTENT_DB_SERVER_APP_DT_PDS_APP_SERVER_H_

#include "app_dt_pds_app.h"
#include "network_rest_server.h"

namespace stonks::app::dt::pds {
class AppServer {
 public:
  AppServer(App app, network::RestServerBuilder rest_server_builder);

 private:
  network::RestServer rest_server_;
};
}  // namespace stonks::app::dt::pds

#endif  // STONKS_APP_DOCKER_TEST_PERSISTENT_DB_SERVER_APP_DT_PDS_APP_SERVER_H_
