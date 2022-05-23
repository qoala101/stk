#ifndef STONKS_NETWORK_CLIENT_SERVER_TYPES_H_
#define STONKS_NETWORK_CLIENT_SERVER_TYPES_H_

#include <cpprest/http_msg.h>

#include <functional>
#include <map>
#include <string>
#include <variant>

#include "type_variant.h"

namespace stonks::network {
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
  std::map<std::string, json::TypeVariant> params{};

  /**
   * @brief Request body description.
   * Client uses it to verify the body of request before sending it.
   * Server uses it to verify the body of request before handling it.
   */
  std::optional<json::TypeVariant> request_body{};

  /**
   * @brief Response body description.
   * Client uses it to verify the body of response when receiving it.
   * Server uses it to verify the body of response before sending it.
   */
  std::optional<json::TypeVariant> response_body{};
} __attribute__((aligned(128)));  // NOLINT(*-magic-numbers)

/**
 * @brief Request handler.
 * Server would only call it if all the endpoint description checks pass.
 */
class EndpointHandler {
 public:
  template <typename T>
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  EndpointHandler(T handler) : handler_{std::move(handler)} {}

  auto operator()(std::map<std::string, std::any> params,
                  std::any request_body) const -> std::any;

 private:
  std::variant<
      std::function<void()>,
      std::function<void(std::map<std::string, std::any>)>,
      std::function<void(std::any)>,
      std::function<void(std::map<std::string, std::any>, std::any)>,

      std::function<std::any()>,
      std::function<std::any(std::map<std::string, std::any>)>,
      std::function<std::any(std::any)>,
      std::function<std::any(std::map<std::string, std::any>, std::any)>>
      handler_{};
};

/**
 * @brief Data used by server to verify and handle request.
 */
struct Endpoint {
  EndpointDesc desc{};
  EndpointHandler handler;
} __attribute__((aligned(128)));  // NOLINT(*-magic-numbers)
}  // namespace stonks::network

#endif  // STONKS_NETWORK_CLIENT_SERVER_TYPES_H_
