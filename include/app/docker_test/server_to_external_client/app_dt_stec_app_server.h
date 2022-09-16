#ifndef STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_APP_SERVER_H_
#define STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_APP_SERVER_H_

#include "app_dt_stec_app.h"
#include "cpp_di_factory.h"
#include "cpp_not_null.h"
#include "network_i_rest_request_receiver.h"
#include "network_types.h"

namespace stonks::app::dt {
class AppServer {
 public:
  AppServer(const cpp::NnSp<App> &app,
            cpp::NnSp<cpp::di::IFactory<network::IRestRequestReceiver>>
                request_receiver_factory,
            network::Uri base_uri);

 private:
  cpp::NnUp<network::IRestRequestReceiver> request_receiver_;
};
}  // namespace stonks::app::dt

#endif  // STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_APP_SERVER_H_
