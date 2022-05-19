#ifndef STONKS_NETWORK_SERVER_H_
#define STONKS_NETWORK_SERVER_H_

#include <memory>
#include <string_view>
#include <vector>

#include "client_server_types.h"

namespace stonks::network {
/**
 * @brief HTTP server wrapper which handles type safety of prams,
 * requests, and responses.
 */
class Server {
 public:
  /**
   * @brief Stops the HTTP server.
   */
  virtual ~Server();

  Server(const Server &) = delete;
  auto operator=(const Server &) -> Server & = delete;

  Server(Server &&) = default;
  auto operator=(Server &&) -> Server & = default;

 protected:
  /**
   * @brief Creates and starts HTTP server on the specified URI which would
   * expose the endpoints and redirect the requests to them.
   */
  explicit Server(std::string_view base_uri, std::vector<Endpoint> endpoints);

 private:
  class Impl;
  std::unique_ptr<Impl> impl_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_SERVER_H_
