#ifndef STONKS_NETWORK_CLIENT_H_
#define STONKS_NETWORK_CLIENT_H_

#include <map>
#include <memory>
#include <string>

#include "endpoint.h"
#include "typed_any.h"
#include "uri.h"

namespace stonks::network {
/**
 * @brief Represents HTTP server client which handles checking of mandatory
 * params and request and response bodies.
 */
class Client {
 public:
  /**
   * @param uri Server URI to which requests are sent.
   */
  explicit Client(const Uri &uri);

  struct ExecuteArgs {
    /**
     * @brief Parameters to be added to request.
     */
    const std::map<std::string, Json> &params{};

    /**
     * @brief Object sent in the body of request.
     */
    const Json &body{};
  };

  /**
   * @brief Sends rest request to the server on specified endpoint.
   * Before sending the request, and after receiving the response, verifies
   * params, body, and response.
   * @throws If response doesn't match endpoint description or rethrows if
   * response is an exception itself.
   */
  // NOLINTNEXTLINE(*-use-nodiscard)
  auto Execute(const EndpointDesc &endpoint,
               const ExecuteArgs &args = {.params = {}, .body = {}}) const
      -> Result;

 private:
  std::string base_uri_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_CLIENT_H_
