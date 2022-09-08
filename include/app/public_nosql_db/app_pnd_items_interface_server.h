#ifndef STONKS_APP_PUBLIC_NOSQL_DB_APP_PND_ITEMS_INTERFACE_SERVER_H_
#define STONKS_APP_PUBLIC_NOSQL_DB_APP_PND_ITEMS_INTERFACE_SERVER_H_

#include <string_view>

#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "network_auto_parsable_request.h"
#include "network_i_rest_request_receiver.h"
#include "nosqldb_i_items_interface.h"
#include "nosqldb_types.h"

namespace stonks::app::pnd {
class ItemsInterfaceServer {
 public:
  explicit ItemsInterfaceServer(std::string_view base_uri);

 private:
  /**
   * @copydoc nosqldb::IItemsInterface::SelectItem
   */
  [[nodiscard]] auto SelectItem(network::AutoParsableRestRequest request) const
      -> cpp::Opt<nosqldb::Item>;

  /**
   * @copydoc nosqldb::IItemsInterface::InsertOrUpdateItem
   */
  void InsertOrUpdateItem(network::AutoParsableRestRequest request) const;

  cpp::NnUp<nosqldb::IItemsInterface> items_interface_;
  cpp::NnUp<network::IRestRequestReceiver> request_receiver_;
};
}  // namespace stonks::app::pnd

#endif  // STONKS_APP_PUBLIC_NOSQL_DB_APP_PND_ITEMS_INTERFACE_SERVER_H_
