#ifndef STONKS_APP_DOCKER_TEST_PERSISTENT_DB_SERVER_APP_DT_PDS_APP_SERVER_H_
#define STONKS_APP_DOCKER_TEST_PERSISTENT_DB_SERVER_APP_DT_PDS_APP_SERVER_H_

#include "app_dt_pds_app.h"
#include "cpp_not_null.h"
#include "di_factory.h"
#include "network_i_rest_request_receiver.h"
#include "network_types.h"

namespace stonks::app::dt::pds {
class AppServer {
 public:
  AppServer(App app, cpp::NnSp<network::IRestRequestReceiver> request_receiver,
            network::Uri base_uri);

 private:
  cpp::NnSp<network::IRestRequestReceiver> request_receiver_;
};
}  // namespace stonks::app::dt::pds

#endif  // STONKS_APP_DOCKER_TEST_PERSISTENT_DB_SERVER_APP_DT_PDS_APP_SERVER_H_
