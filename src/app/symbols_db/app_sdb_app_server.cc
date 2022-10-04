#include "app_sdb_app_server.h"

#include <memory>
#include <utility>

#include "app_endpoints.h"
#include "cpp_not_null.h"
#include "network_auto_parsable.h"
#include "network_auto_parsable_request.h"
#include "network_json_basic_conversions.h"
#include "network_rest_server_builder.h"
#include "not_null.hpp"

namespace stonks::app::sdb {
auto AppServer::InsertOrUpdateSymbolInfo(cpp::NnSp<App> app) {
  return
      [app = std::move(app)](network::AutoParsableRestRequest request) mutable {
        app->InsertOrUpdateSymbolInfo(request.Body());
      };
}

auto AppServer::InsertSymbolPriceRecord(cpp::NnSp<App> app) {
  return
      [app = std::move(app)](network::AutoParsableRestRequest request) mutable {
        app->InsertSymbolPriceRecord(request.Body());
      };
}

AppServer::AppServer(App app, network::RestServerBuilder rest_server_builder)
    : rest_server_{[&app, &rest_server_builder]() {
        auto shared_app = cpp::MakeNnSp<App>(std::move(app));
        return rest_server_builder
            .Handling(endpoints::InsertOrUpdateSymbolInfo(),
                      InsertOrUpdateSymbolInfo(shared_app))
            .Handling(endpoints::InsertSymbolPriceRecord(),
                      InsertSymbolPriceRecord(std::move(shared_app)))
            .Start();
      }()} {}
}  // namespace stonks::app::sdb