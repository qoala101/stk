#include "app_dt_pds_app_server.h"

#include <utility>

#include "app_dt_pds_app_endpoints.h"
#include "network_rest_server_builder.h"

namespace stonks::app::dt::pds {
AppServer::AppServer(App app, network::RestServerBuilder rest_server_builder)
    : rest_server_{rest_server_builder
                       .Handling(endpoints::RecordSymbolPrice(),
                                 [app = std::move(app)](auto request) mutable {
                                   app.RecordSymbolPrice(request.Body());
                                 })
                       .Start()} {}
}  // namespace stonks::app::dt::pds