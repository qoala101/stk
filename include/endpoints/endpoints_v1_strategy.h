#ifndef STONKS_ENDPOINTS_ENDPOINTS_V1_STRATEGY_H_
#define STONKS_ENDPOINTS_ENDPOINTS_V1_STRATEGY_H_

#include "endpoint.h"

namespace stonks::endpoints {
class V1Strategy {
 public:
  [[nodiscard]] static auto Run() -> network::EndpointDesc;
};
}  // namespace stonks::endpoints

#endif  // STONKS_ENDPOINTS_ENDPOINTS_V1_STRATEGY_H_
