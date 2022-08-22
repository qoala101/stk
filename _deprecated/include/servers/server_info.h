#ifndef STONKS_SERVERS_SERVER_INFO_H_
#define STONKS_SERVERS_SERVER_INFO_H_

#include "cpp_not_null.h"

#include "endpoint.h"
#include "server.h"
#include "stonks_info.h"

namespace stonks::server {
class Info {
 public:
  static constexpr auto kEndpoint = "/Info";

  explicit Info(int port, cpp::Sp<stonks::Info> entity);

 private:
  [[nodiscard]] auto GetSymbols() -> network::Endpoint;
  [[nodiscard]] auto GetStrategyNames() -> network::Endpoint;
  [[nodiscard]] auto GetPriceTicks() -> network::Endpoint;

  network::Server server_;
  cpp::Sp<stonks::Info> entity_{};
};
}  // namespace stonks::server

#endif  // STONKS_SERVERS_SERVER_INFO_H_
