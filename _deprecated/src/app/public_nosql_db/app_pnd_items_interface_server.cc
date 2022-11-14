#include "app_pnd_items_interface_server.h"

#include <memory>

#include "app_pnd_items_interface_endpoints.h"
#include "app_pnd_json_user_conversions.h"
#include "cpp_not_null.h"
#include "network_auto_parsable.h"
#include "network_auto_parsable_request.h"
#include "network_json_common_conversions.h"
#include "network_rest_server_builder.h"

namespace stonks::app::pnd {
ItemsInterfaceServer::ItemsInterfaceServer(
    const cpp::NnSp<nosqldb::IItemsInterface> &items_interface,
    network::RestServerBuilder rest_server_builder)
    : rest_server_{
          rest_server_builder
              .Handling(endpoints::SelectItem(),
                        [items_interface](auto request) {
                          return items_interface->SelectItem(
                              request.Param("table"), request.Param("key"));
                        })
              .Handling(endpoints::InsertOrUpdateItem(),
                        [items_interface](auto request) {
                          items_interface->InsertOrUpdateItem(
                              request.Param("table"), request.Param("item"));
                        })
              .Start()} {}
}  // namespace stonks::app::pnd