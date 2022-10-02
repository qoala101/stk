#ifndef STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_APP_SERVER_H_
#define STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_APP_SERVER_H_

#include "app_dt_stec_app.h"
#include "cpp_not_null.h"
#include "network_i_rest_request_receiver.h"
#include "network_rest_server.h"
#include "network_types.h"

namespace stonks::app::dt::stec {
class AppServer {
 public:
  AppServer(App app, cpp::NnUp<network::IRestRequestReceiver> request_receiver,
            network::Uri base_uri);

 private:
  network::RestServer rest_server_;
};
}  // namespace stonks::app::dt::stec

#endif  // STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_APP_SERVER_H_
