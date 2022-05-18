#ifndef STONKS_NETWORK_SERVER_H_
#define STONKS_NETWORK_SERVER_H_

#include <memory>
#include <string_view>
#include <vector>

#include "client_server_types.h"

namespace stonks {
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

  Server(Server &&) = default;
  auto operator=(Server &&) -> Server & = default;

  Server(const Server &) = delete;
  auto operator=(const Server &) -> Server & = delete;

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
}  // namespace stonks

#endif  // STONKS_NETWORK_SERVER_H_
