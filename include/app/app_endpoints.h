#ifndef STONKS_APP_APP_ENDPOINTS_H_
#define STONKS_APP_APP_ENDPOINTS_H_

#include "network_typed_endpoint.h"

namespace stonks::app::endpoints {
/**
 * @copydoc sdb::App::InsertOrUpdateSymbolInfo
 */
[[nodiscard]] auto InsertOrUpdateSymbolInfo() -> const network::TypedEndpoint &;

/**
 * @copydoc sdb::App::InsertSymbolPriceRecord
 */
[[nodiscard]] auto InsertSymbolPriceRecord() -> const network::TypedEndpoint &;
}  // namespace stonks::app::endpoints

#endif  // STONKS_APP_APP_ENDPOINTS_H_
