#ifndef STONKS_SERVERS_SERVER_V1_STRATEGY_H_
#define STONKS_SERVERS_SERVER_V1_STRATEGY_H_

#include <memory>

#include "server.h"
#include "v1_strategy.h"

namespace stonks {
class V1StrategyServer {
 public:
  static constexpr auto kEndpoint = "/V1Strategy";

  explicit V1StrategyServer(int port, std::shared_ptr<V1Strategy> entity);

 private:
  [[nodiscard]] auto Run() -> network::Endpoint;

  network::Server server_;
  std::shared_ptr<V1Strategy> entity_{};
};
}  // namespace stonks

#endif  // STONKS_SERVERS_SERVER_V1_STRATEGY_H_
