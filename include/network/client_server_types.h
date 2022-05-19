#ifndef STONKS_NETWORK_CLIENT_SERVER_TYPES_H_
#define STONKS_NETWORK_CLIENT_SERVER_TYPES_H_

#include <cpprest/http_msg.h>

#include <functional>
#include <map>
#include <optional>
#include <string>

#include "any_types.h"

namespace stonks {
/**
 * @brief Description of REST endpoint.
 * Used by Client to send the request, and by Server to handle it.
 */
struct EndpointDesc {
  /**
   * @brief HTTP method.
   */
  web::http::method method{};

  /**
   * @brief URI relative to the base URI.
   * Client sends the request on this URI.
   * Server redirects the request on this URI to the corresponding
   * handler.
   */
  std::string relative_uri{};

  /**
   * @brief List of request parameter descriptions.
   * Client uses it to verify the params before sending the request.
   * Server uses it to verify the params before handling the request.
   */
  std::map<std::string, json::AnyDesc> params{};

  /**
   * @brief Request body description.
   * Client uses it to verify the body of request before sending it.
   * Server uses it to verify the body of request before handling it.
   */
  std::optional<json::AnyDesc> request_body{};

  /**
   * @brief Response body description.
   * Client uses it to verify the body of response when receiving it.
   * Server uses it to verify the body of response before sending it.
   */
  std::optional<json::AnyDesc> response_body{};
} __attribute__((aligned(128)));  // NOLINT(*-magic-numbers)

/**
 * @brief Request handler.
 * Server would only call it if all the endpoint description checks pass.
 */
using EndpointHandler = std::function<std::any /*response_body*/ (
    std::map<std::string, json::Any> /*params*/, json::Any /*request_body*/)>;

struct Endpoint {
  EndpointDesc desc{};
  EndpointHandler handler{};
} __attribute__((aligned(128)));  // NOLINT(*-magic-numbers)
}  // namespace stonks

#endif  // STONKS_NETWORK_CLIENT_SERVER_TYPES_H_
