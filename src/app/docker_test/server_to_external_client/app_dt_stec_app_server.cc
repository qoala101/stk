#include "app_dt_stec_app_server.h"

#include <utility>

#include "app_dt_stec_app_endpoints.h"
#include "cpp_not_null.h"
#include "network_auto_parsable.h"
#include "network_auto_parsable_request.h"
#include "network_json_common_conversions.h"
#include "network_rest_server_builder.h"

namespace stonks::app::dt::stec {
AppServer::AppServer(App app,
                     cpp::NnUp<network::IRestRequestReceiver> request_receiver,
                     network::Uri base_uri)
    : request_receiver_{
          network::RestServerBuilder{std::move(request_receiver)}
              .On(std::move(base_uri))
              .Handling(endpoints::GetAveragePrice(),
                        [app = std::move(app)](
                            network::AutoParsableRestRequest request) {
                          return app.GetAveragePrice(request.Param("symbol"));
                        })
              .Start()} {}
}  // namespace stonks::app::dt::stec