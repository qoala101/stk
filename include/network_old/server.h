#ifndef STONKS_NETWORK_SERVER_H_
#define STONKS_NETWORK_SERVER_H_

#include "ccutils_not_null.h"
#include <vector>

#include "endpoint.h"
#include "uri.h"

namespace stonks::network {
/**
 * @brief Wrapper of HTTP server which simplifies exposing of C++ methods
 * as REST endpoints.
 * Client provides list of supported endpoints and their handlers
 * and Server redirects corresponding requests there.
 */
class Server {
 public:
  /**
   * @brief Creates and starts HTTP server on the specified URI which would
   * expose the endpoints and redirect the requests to them.
   */
  explicit Server(const LocalUri &uri, std::vector<Endpoint> endpoints);

  Server(const Server &) = delete;
  Server(Server &&) noexcept;

  auto operator=(const Server &) -> Server & = delete;
  auto operator=(Server &&) noexcept -> Server &;

  /**
   * @brief Stops the HTTP server.
   */
  ~Server();

 private:
  class Impl;
  ccutils::Up<Impl> impl_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_SERVER_H_
