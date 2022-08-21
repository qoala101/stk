#ifndef STONKS_ENDPOINTS_ENDPOINTS_FINANCE_DB_H_
#define STONKS_ENDPOINTS_ENDPOINTS_FINANCE_DB_H_

#include "endpoint.h"

namespace stonks::endpoints {
class FinanceDb {
 public:
  [[nodiscard]] static auto SelectAssets() -> network::EndpointDesc;
  [[nodiscard]] static auto UpdateAssets() -> network::EndpointDesc;
  [[nodiscard]] static auto SelectSymbols() -> network::EndpointDesc;
  [[nodiscard]] static auto SelectSymbolsInfo() -> network::EndpointDesc;
  [[nodiscard]] static auto UpdateSymbolsInfo() -> network::EndpointDesc;
  [[nodiscard]] static auto SelectSymbolPriceTicks() -> network::EndpointDesc;
  [[nodiscard]] static auto InsertSymbolPriceTick() -> network::EndpointDesc;
};
}  // namespace stonks::endpoints

#endif  // STONKS_ENDPOINTS_ENDPOINTS_FINANCE_DB_H_
