#ifndef STONKS_NETWORK_OLD_CLIENT_H_
#define STONKS_NETWORK_OLD_CLIENT_H_

#include <map>
#include "ccutils_not_null.h"
#include <string>

#include "endpoint.h"

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
     * @brief Object sent as the body of request.
     */
    const Json &body{};
  };

  /**
   * @brief Sends rest request to the specified endpoint.
   * @throws If response doesn't match endpoint description or Rethrows if
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

#endif  // STONKS_NETWORK_OLD_CLIENT_H_
