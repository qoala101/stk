#ifndef STONKS_SERVERS_SERVER_V1_STRATEGY_H_
#define STONKS_SERVERS_SERVER_V1_STRATEGY_H_

#include "ccutils_not_null.h"

#include "endpoint.h"
#include "server.h"
#include "v1_strategy.h"

namespace stonks::server {
class V1Strategy {
 public:
  static constexpr auto kEndpoint = "/V1Strategy";

  explicit V1Strategy(int port, ccutils::Sp<stonks::V1Strategy> entity);

 private:
  [[nodiscard]] auto Run() -> network::Endpoint;

  network::Server server_;
  ccutils::Sp<stonks::V1Strategy> entity_{};
};
}  // namespace stonks::server

#endif  // STONKS_SERVERS_SERVER_V1_STRATEGY_H_
