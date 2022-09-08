#include "app_pnd_items_interface_server.h"

#include <functional>
#include <memory>
#include <optional>
#include <string_view>

#include "app_pnd_items_interface_endpoints.h"
#include "app_pnd_json_user_conversions.h"
#include "aws_dynamodb_factory.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "network_auto_parsable.h"
#include "network_auto_parsable_request.h"
#include "network_i_factory.h"
#include "network_json_basic_conversions.h"
#include "network_json_common_conversions.h"
#include "network_rest_server.h"
#include "nosqldb_types.h"
#include "restsdk_factory.h"

namespace stonks::app::pnd {
ItemsInterfaceServer::ItemsInterfaceServer(std::string_view base_uri)
    : items_interface_{aws::dynamodb::Factory{}.CreateItemsInterface()},
      request_receiver_{
          network::RestServer{cpp::MakeNnUp<restsdk::Factory>()}
              .On(base_uri.data())
              .Handling(
                  endpoints::SelectItem(),
                  std::bind_front(&ItemsInterfaceServer::SelectItem, this))
              .Handling(endpoints::InsertOrUpdateItem(),
                        std::bind_front(
                            &ItemsInterfaceServer::InsertOrUpdateItem, this))
              .Start()} {}

auto ItemsInterfaceServer::SelectItem(
    network::AutoParsableRestRequest request) const -> cpp::Opt<nosqldb::Item> {
  return items_interface_->SelectItem(request.Param("table"),
                                      request.Param("key"));
}

void ItemsInterfaceServer::InsertOrUpdateItem(
    network::AutoParsableRestRequest request) const {
  items_interface_->InsertOrUpdateItem(request.Param("table"),
                                       request.Param("item"));
}
}  // namespace stonks::app::pnd