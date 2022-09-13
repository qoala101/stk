#include "app_pnd_items_interface_server.h"

#include <memory>
#include <string_view>
#include <utility>

#include "app_pnd_items_interface_endpoints.h"
#include "app_pnd_json_user_conversions.h"
#include "cpp_not_null.h"
#include "network_auto_parsable.h"
#include "network_auto_parsable_request.h"
#include "network_i_factory.h"
#include "network_json_basic_conversions.h"
#include "network_json_common_conversions.h"
#include "network_rest_server.h"

namespace stonks::app::pnd {
ItemsInterfaceServer::ItemsInterfaceServer(
    const cpp::NnSp<nosqldb::IItemsInterface> &items_interface,
    cpp::NnUp<network::IFactory> network_factory, std::string_view base_uri)
    : request_receiver_{
          network::RestServer{std::move(network_factory)}
              .On(base_uri.data())
              .Handling(
                  endpoints::SelectItem(),
                  [items_interface](network::AutoParsableRestRequest request) {
                    return items_interface->SelectItem(request.Param("table"),
                                                       request.Param("key"));
                  })
              .Handling(
                  endpoints::InsertOrUpdateItem(),
                  [items_interface](network::AutoParsableRestRequest request) {
                    items_interface->InsertOrUpdateItem(request.Param("table"),
                                                        request.Param("item"));
                  })
              .Start()} {}
}  // namespace stonks::app::pnd