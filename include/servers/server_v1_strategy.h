#ifndef STONKS_SERVERS_SERVER_V1_STRATEGY_H_
#define STONKS_SERVERS_SERVER_V1_STRATEGY_H_

#include <memory>

#include "server.h"
#include "v1_strategy.h"

namespace stonks::server {
class V1Strategy {
 public:
  static constexpr auto kEndpoint = "/V1Strategy";

  explicit V1Strategy(int port, std::shared_ptr<stonks::V1Strategy> entity);

 private:
  [[nodiscard]] auto Run() -> network::Endpoint;

  network::Server server_;
  std::shared_ptr<stonks::V1Strategy> entity_{};
};
}  // namespace stonks::server

#endif  // STONKS_SERVERS_SERVER_V1_STRATEGY_H_
