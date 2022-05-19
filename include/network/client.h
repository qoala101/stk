#ifndef STONKS_NETWORK_CLIENT_H_
#define STONKS_NETWORK_CLIENT_H_

#include <map>
#include <memory>
#include <string>
#include <string_view>

#include "client_server_types.h"

namespace stonks {
/**
 * @brief Represents HTTP server client which handles type safety of prams,
 * requests, and responses.
 */
class Client {
 public:
  virtual ~Client();

  Client(const Client &) = delete;
  auto operator=(const Client &) -> Client & = delete;

  Client(Client &&) = default;
  auto operator=(Client &&) -> Client & = default;

 protected:
  /**
   * @param base_uri Base server URI to which requests are sent.
   */
  explicit Client(std::string_view base_uri);

  /**
   * @brief Sends rest request to the server on specified endpoint.
   * Before sending the request, and after receiving the response, verifies
   * params, body, and response.
   * @throws If response doesn't match endpoint description or response is an
   * exception itself.
   */
  // NOLINTNEXTLINE(*-use-nodiscard)
  auto Execute(const EndpointDesc &endpoint,
               const std::map<std::string, json::Any> &params = {},
               const stonks::json::Any &request_body = {}) const -> std::any;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_{};
};
}  // namespace stonks

#endif  // STONKS_NETWORK_CLIENT_H_
