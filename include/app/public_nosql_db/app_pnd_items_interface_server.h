#ifndef STONKS_APP_PUBLIC_NOSQL_DB_APP_PND_ITEMS_INTERFACE_SERVER_H_
#define STONKS_APP_PUBLIC_NOSQL_DB_APP_PND_ITEMS_INTERFACE_SERVER_H_

#include <string_view>

#include "cpp_not_null.h"
#include "network_i_factory.h"
#include "network_i_rest_request_receiver.h"
#include "nosqldb_i_items_interface.h"

namespace stonks::app::pnd {
class ItemsInterfaceServer {
 public:
  explicit ItemsInterfaceServer(
      const cpp::NnSp<nosqldb::IItemsInterface> &items_interface,
      cpp::NnUp<network::IFactory> network_factory, std::string_view base_uri);

 private:
  cpp::NnUp<network::IRestRequestReceiver> request_receiver_;
};
}  // namespace stonks::app::pnd

#endif  // STONKS_APP_PUBLIC_NOSQL_DB_APP_PND_ITEMS_INTERFACE_SERVER_H_
