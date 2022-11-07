#include "app_dt_stec_app_server.h"

#include <utility>

#include "app_dt_stec_app_endpoints.h"
#include "network_auto_parsable.h"
#include "network_auto_parsable_request.h"
#include "network_json_common_conversions.h"
#include "network_rest_server_builder.h"

namespace stonks::app::dt::stec {
AppServer::AppServer(App app, network::RestServerBuilder rest_server_builder)
    : rest_server_{rest_server_builder
                       .Handling(endpoints::GetAveragePrice(),
                                 [app = std::move(app)](auto request) {
                                   return app.GetAveragePrice(
                                       request.Param("symbol"));
                                 })
                       .Start()} {}
}  // namespace stonks::app::dt::stec