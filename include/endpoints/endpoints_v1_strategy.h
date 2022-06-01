#ifndef STONKS_ENDPOINTS_ENDPOINTS_V1_STRATEGY_H_
#define STONKS_ENDPOINTS_ENDPOINTS_V1_STRATEGY_H_

#include "endpoint.h"

namespace stonks {
class V1StrategyEndpoints {
 public:
  [[nodiscard]] static auto Run() -> network::EndpointDesc;
};
}  // namespace stonks

#endif  // STONKS_ENDPOINTS_ENDPOINTS_V1_STRATEGY_H_
