#ifndef STONKS_SERVICES_V2_V1_STRATEGY_SERVER_H_
#define STONKS_SERVICES_V2_V1_STRATEGY_SERVER_H_

#include <string_view>

#include "server.h"
#include "v1_strategy.h"

namespace stonks {
class V1StrategyServer {
 public:
  explicit V1StrategyServer(std::string_view base_uri);

  static constexpr auto kEndpoint = "/V1Strategy";

 private:
  network::Server server_;
  V1Strategy strategy_{};
};
}  // namespace stonks

#endif  // STONKS_SERVICES_V2_V1_STRATEGY_SERVER_H_
