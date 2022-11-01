#ifndef STONKS_APP_PUBLIC_NOSQL_DB_APP_PND_ITEMS_INTERFACE_ENDPOINTS_H_
#define STONKS_APP_PUBLIC_NOSQL_DB_APP_PND_ITEMS_INTERFACE_ENDPOINTS_H_

#include "network_typed_endpoint.h"

namespace stonks::app::pnd::endpoints {
/**
 * @copydoc nosqldb::IItemsInterface::SelectItem
 */
[[nodiscard]] auto SelectItem() -> network::TypedEndpoint;

/**
 * @copydoc nosqldb::IItemsInterface::InsertOrUpdateItem
 */
[[nodiscard]] auto InsertOrUpdateItem() -> network::TypedEndpoint;
}  // namespace stonks::app::pnd::endpoints

#endif  // STONKS_APP_PUBLIC_NOSQL_DB_APP_PND_ITEMS_INTERFACE_ENDPOINTS_H_
