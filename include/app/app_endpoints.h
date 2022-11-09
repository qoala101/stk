#ifndef STONKS_APP_APP_ENDPOINTS_H_
#define STONKS_APP_APP_ENDPOINTS_H_

#include "network_typed_endpoint.h"

namespace stonks::app::endpoints {
/**
 * @copydoc sdb::App::SelectAssets
 */
auto SelectAssets [[nodiscard]] () -> const network::TypedEndpoint &;

/**
 * @copydoc sdb::App::UpdateAssets
 */
auto UpdateAssets [[nodiscard]] () -> const network::TypedEndpoint &;

/**
 * @copydoc sdb::App::SelectSymbolsWithPriceRecords
 */
auto SelectSymbolsWithPriceRecords [[nodiscard]] ()
-> const network::TypedEndpoint &;

/**
 * @copydoc sdb::App::SelectSymbolInfo
 */
auto SelectSymbolInfo [[nodiscard]] () -> const network::TypedEndpoint &;

/**
 * @copydoc sdb::App::SelectSymbolsInfo
 */
auto SelectSymbolsInfo [[nodiscard]] () -> const network::TypedEndpoint &;

/**
 * @copydoc sdb::App::UpdateSymbolsInfo
 */
auto UpdateSymbolsInfo [[nodiscard]] () -> const network::TypedEndpoint &;

/**
 * @copydoc sdb::App::SelectSymbolPriceRecords
 */
auto SelectSymbolPriceRecords [[nodiscard]] ()
-> const network::TypedEndpoint &;

/**
 * @copydoc sdb::App::InsertSymbolPriceRecord
 */
auto InsertSymbolPriceRecord [[nodiscard]] () -> const network::TypedEndpoint &;

/**
 * @copydoc sdb::App::DeleteSymbolPriceRecords
 */
auto DeleteSymbolPriceRecords [[nodiscard]] ()
-> const network::TypedEndpoint &;
}  // namespace stonks::app::endpoints

#endif  // STONKS_APP_APP_ENDPOINTS_H_
