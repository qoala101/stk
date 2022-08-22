#ifndef STONKS_STONKS_ENDPOINTS_STONKS_ENDPOINTS_DB_H_
#define STONKS_STONKS_ENDPOINTS_STONKS_ENDPOINTS_DB_H_

#include "network_types.h"

/**
 * @file REST endpoints for public DB API.
 */
namespace stonks::endpoints::db {
[[nodiscard]] auto SelectAssets() -> const network::Endpoint &;
[[nodiscard]] auto UpdateAssets() -> const network::Endpoint &;
[[nodiscard]] auto SelectSymbols() -> const network::Endpoint &;
[[nodiscard]] auto SelectSymbolsInfo() -> const network::Endpoint &;
[[nodiscard]] auto UpdateSymbolsInfo() -> const network::Endpoint &;
[[nodiscard]] auto SelectSymbolPriceTicks() -> const network::Endpoint &;
[[nodiscard]] auto InsertSymbolPriceTick() -> const network::Endpoint &;
}  // namespace stonks::endpoints::db

#endif  // STONKS_STONKS_ENDPOINTS_STONKS_ENDPOINTS_DB_H_
