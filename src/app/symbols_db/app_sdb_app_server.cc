#include "app_sdb_app_server.h"

#include <utility>
#include <memory>

#include "app_endpoints.h"
#include "cpp_not_null.h"
#include "network_auto_parsable.h"
#include "network_auto_parsable_request.h"
#include "network_json_basic_conversions.h"
#include "network_rest_server_builder.h"
#include "not_null.hpp"

namespace stonks::app::sdb {
AppServer::AppServer(App app,
                     cpp::NnUp<network::IRestRequestReceiver> request_receiver,
                     network::Uri base_uri)
    : request_receiver_{[&app, &request_receiver, &base_uri]() {
        auto shared_app = cpp::MakeNnSp<App>(std::move(app));

        return network::RestServerBuilder{std::move(request_receiver)}
            .On(std::move(base_uri))
            .Handling(
                endpoints::InsertSymbolPriceRecord(),
                [shared_app](network::AutoParsableRestRequest request) mutable {
                  shared_app->InsertSymbolPriceRecord(request.Body());
                })
            .Start();
      }()} {}
}  // namespace stonks::app::sdb