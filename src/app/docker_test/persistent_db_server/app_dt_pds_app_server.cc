#include "app_dt_pds_app_server.h"

#include <utility>

#include "app_dt_pds_app_endpoints.h"
#include "cpp_not_null.h"
#include "network_auto_parsable.h"
#include "network_auto_parsable_request.h"
#include "network_json_basic_conversions.h"
#include "network_rest_server_builder.h"

namespace stonks::app::dt::pds {
AppServer::AppServer(App app,
                     cpp::NnSp<network::IRestRequestReceiver> request_receiver,
                     network::Uri base_uri)
    : request_receiver_{
          network::RestServerBuilder{std::move(request_receiver)}
              .On(std::move(base_uri))
              .Handling(endpoints::RecordSymbolPrice(),
                        [app = std::move(app)](
                            network::AutoParsableRestRequest request) mutable {
                          app.RecordSymbolPrice(request.Body());
                        })
              .Start()} {}
}  // namespace stonks::app::dt::pds