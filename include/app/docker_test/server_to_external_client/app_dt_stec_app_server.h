#ifndef STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_APP_SERVER_H_
#define STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_APP_SERVER_H_

#include "app_dt_stec_app.h"
#include "cpp_not_null.h"
#include "di_factory.h"
#include "network_i_rest_request_receiver.h"
#include "network_types.h"

namespace stonks::app::dt::stec {
class AppServer {
 public:
  AppServer(App app, cpp::NnSp<network::IRestRequestReceiver> request_receiver,
            network::Uri base_uri);

 private:
  cpp::NnSp<network::IRestRequestReceiver> request_receiver_;
};
}  // namespace stonks::app::dt::stec

#endif  // STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_APP_SERVER_H_
