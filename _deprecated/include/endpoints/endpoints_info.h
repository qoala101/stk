#ifndef STONKS_ENDPOINTS_ENDPOINTS_INFO_H_
#define STONKS_ENDPOINTS_ENDPOINTS_INFO_H_

#include "endpoint.h"

namespace stonks::endpoints {
class Info {
 public:
  [[nodiscard]] static auto GetSymbols() -> network::EndpointDesc;
  [[nodiscard]] static auto GetStrategyNames() -> network::EndpointDesc;
  [[nodiscard]] static auto GetPriceTicks() -> network::EndpointDesc;
};
}  // namespace stonks::endpoints

#endif  // STONKS_ENDPOINTS_ENDPOINTS_INFO_H_
