#include "app_dt_stec_app_server.h"

#include <memory>
#include <utility>

#include "app_dt_stec_app_endpoints.h"
#include "cpp_not_null.h"
#include "network_auto_parsable.h"
#include "network_auto_parsable_request.h"
#include "network_json_common_conversions.h"
#include "network_rest_server.h"

namespace stonks::app::dt {
AppServer::AppServer(const cpp::NnSp<App> &app,
                     cpp::NnSp<cpp::di::IFactory<network::IRestRequestReceiver>>
                         request_receiver_factory,
                     network::Uri base_uri)
    : request_receiver_{
          network::RestServer{std::move(request_receiver_factory)}
              .On(std::move(base_uri))
              .Handling(endpoints::GetAveragePrice(),
                        [app](network::AutoParsableRestRequest request) {
                          return app->GetAveragePrice(request.Param("symbol"));
                        })
              .Start()} {}
}  // namespace stonks::app::dt