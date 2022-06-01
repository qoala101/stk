#ifndef STONKS_ENDPOINTS_ENDPOINTS_BINANCE_H_
#define STONKS_ENDPOINTS_ENDPOINTS_BINANCE_H_

#include "endpoint.h"

namespace stonks {
class BinanceEndpoints {
 public:
  [[nodiscard]] static auto GetExchangeInfo() -> network::EndpointDesc;
};
}  // namespace stonks

#endif  // STONKS_ENDPOINTS_ENDPOINTS_BINANCE_H_
