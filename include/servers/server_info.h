#ifndef STONKS_SERVERS_SERVER_INFO_H_
#define STONKS_SERVERS_SERVER_INFO_H_

#include "endpoint.h"
#include "server.h"
#include "stonks_info.h"

namespace stonks {
class InfoServer {
 public:
  static constexpr auto kEndpoint = "/Info";

  explicit InfoServer(std::string_view base_uri);

 private:
  [[nodiscard]] auto GetSymbols() -> network::Endpoint;
  [[nodiscard]] auto GetStrategyNames() -> network::Endpoint;
  [[nodiscard]] auto GetPriceTicks() -> network::Endpoint;

  network::Server server_;
  Info info_{};
};
}  // namespace stonks

#endif  // STONKS_SERVERS_SERVER_INFO_H_
