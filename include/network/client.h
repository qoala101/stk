#ifndef STONKS_NETWORK_CLIENT_H_
#define STONKS_NETWORK_CLIENT_H_

#include <map>
#include <memory>
#include <string>
#include <string_view>

#include "endpoint.h"
#include "typed_any.h"
#include "uri.h"

namespace stonks::network {
/**
 * @brief Represents HTTP server client which handles type safety of prams,
 * requests, and responses.
 */
class Client {
 public:
  /**
   * @param uri Server URI to which requests are sent.
   */
  explicit Client(const Uri &uri);

  Client(const Client &) = delete;
  Client(Client &&) noexcept;

  auto operator=(const Client &) -> Client & = delete;
  auto operator=(Client &&) noexcept -> Client &;

  ~Client();

  /**
   * @brief Sends rest request to the server on specified endpoint.
   * Before sending the request, and after receiving the response, verifies
   * params, body, and response.
   * @throws If response doesn't match endpoint description or response is an
   * exception itself.
   */
  // NOLINTNEXTLINE(*-use-nodiscard)
  auto Execute(const EndpointDesc &endpoint) const -> Result;

  // NOLINTNEXTLINE(*-use-nodiscard)
  auto Execute(const EndpointDesc &endpoint,
               const std::map<std::string, json::TypedAny> &params) const
      -> Result;

  // NOLINTNEXTLINE(*-use-nodiscard)
  auto Execute(const EndpointDesc &endpoint,
               const json::TypedAny &request_body) const -> Result;

  // NOLINTNEXTLINE(*-use-nodiscard)
  auto Execute(const EndpointDesc &endpoint,
               const std::map<std::string, json::TypedAny> &params,
               const json::TypedAny &request_body) const -> Result;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_CLIENT_H_
