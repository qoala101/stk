#ifndef STONKS_SERVERS_SERVER_V1_STRATEGY_H_
#define STONKS_SERVERS_SERVER_V1_STRATEGY_H_

#include <string_view>

#include "endpoint.h"
#include "server.h"
#include "v1_strategy.h"

namespace stonks {
class V1StrategyServer {
 public:
  static constexpr auto kEndpoint = "/V1Strategy";

  explicit V1StrategyServer(std::string_view base_uri);

 private:
  [[nodiscard]] auto Run() -> network::Endpoint;

  network::Server server_;
  V1Strategy strategy_{};
};
}  // namespace stonks

#endif  // STONKS_SERVERS_SERVER_V1_STRATEGY_H_
