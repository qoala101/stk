#ifndef STONKS_APP_PUBLIC_NOSQL_DB_APP_PND_ITEMS_INTERFACE_SERVER_H_
#define STONKS_APP_PUBLIC_NOSQL_DB_APP_PND_ITEMS_INTERFACE_SERVER_H_

#include "cpp_not_null.h"
#include "network_i_rest_request_receiver.h"
#include "network_rest_server.h"
#include "network_types.h"
#include "nosqldb_i_items_interface.h"

namespace stonks::app::pnd {
class ItemsInterfaceServer {
 public:
  ItemsInterfaceServer(
      const cpp::NnSp<nosqldb::IItemsInterface> &items_interface,
      cpp::NnUp<network::IRestRequestReceiver> request_receiver,
      network::Uri base_uri);

 private:
  network::RestServer rest_server_;
};
}  // namespace stonks::app::pnd

#endif  // STONKS_APP_PUBLIC_NOSQL_DB_APP_PND_ITEMS_INTERFACE_SERVER_H_
