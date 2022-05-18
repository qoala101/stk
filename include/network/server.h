#ifndef STONKS_NETWORK_SERVER_H_
#define STONKS_NETWORK_SERVER_H_

#include <memory>
#include <string_view>
#include <vector>

#include "client_server_types.h"

namespace stonks {
class Server {
 public:
  virtual ~Server();

 protected:
  explicit Server(std::string_view base_uri, std::vector<EndpointDesc> endpoints);

 private:
  class Impl;
  std::unique_ptr<Impl> impl_{};
};
}  // namespace stonks

#endif  // STONKS_NETWORK_SERVER_H_
