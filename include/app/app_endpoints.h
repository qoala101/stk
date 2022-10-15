#ifndef STONKS_APP_APP_ENDPOINTS_H_
#define STONKS_APP_APP_ENDPOINTS_H_

#include "network_typed_endpoint.h"

namespace stonks::app::endpoints {
/**
 * @copydoc sdb::App::SelectAssets
 */
[[nodiscard]] auto SelectAssets() -> const network::TypedEndpoint &;

/**
 * @copydoc sdb::App::UpdateAssets
 */
[[nodiscard]] auto UpdateAssets() -> const network::TypedEndpoint &;

/**
 * @copydoc sdb::App::SelectSymbolsWithPriceRecords
 */
[[nodiscard]] auto SelectSymbolsWithPriceRecords()
    -> const network::TypedEndpoint &;

/**
 * @copydoc sdb::App::SelectSymbolInfo
 */
[[nodiscard]] auto SelectSymbolInfo() -> const network::TypedEndpoint &;

/**
 * @copydoc sdb::App::SelectSymbolsInfo
 */
[[nodiscard]] auto SelectSymbolsInfo() -> const network::TypedEndpoint &;

/**
 * @copydoc sdb::App::UpdateSymbolsInfo
 */
[[nodiscard]] auto UpdateSymbolsInfo() -> const network::TypedEndpoint &;

/**
 * @copydoc sdb::App::SelectSymbolPriceRecords
 */
[[nodiscard]] auto SelectSymbolPriceRecords() -> const network::TypedEndpoint &;

/**
 * @copydoc sdb::App::InsertSymbolPriceRecord
 */
[[nodiscard]] auto InsertSymbolPriceRecord() -> const network::TypedEndpoint &;

/**
 * @copydoc sdb::App::DeleteSymbolPriceRecords
 */
[[nodiscard]] auto DeleteSymbolPriceRecords() -> const network::TypedEndpoint &;
}  // namespace stonks::app::endpoints

#endif  // STONKS_APP_APP_ENDPOINTS_H_
