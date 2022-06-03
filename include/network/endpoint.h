#ifndef STONKS_NETWORK_ENDPOINT_H_
#define STONKS_NETWORK_ENDPOINT_H_

#include <cpprest/http_msg.h>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <variant>

#include "any.h"
#include "any_map.h"
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
};

using Result = json::Any;
using Params = json::AnyMap;
using Body = json::Any;

using NoResult = std::function<void()>;
using NoResultTakesParams = std::function<void(Params)>;
using NoResultTakesBody = std::function<void(Body)>;
using NoResultTakesParamsAndBody = std::function<void(Params, Body)>;

using HasResult = std::function<Result()>;
using HasResultTakesParams = std::function<Result(Params)>;
using HasResultTakesBody = std::function<Result(Body)>;
using HasResultTakesParamsAndBody = std::function<Result(Params, Body)>;

/**
 * @brief Request handler.
 * Server would only call it if all the endpoint description checks pass.
 */
class EndpointHandler {
 public:
  template <typename T>
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  EndpointHandler(T handler) : handler_{std::move(handler)} {}

  auto operator()(Params params,
                  Body request_body) const -> Result;

 private:
  std::variant<NoResult, NoResultTakesParams, NoResultTakesBody,
               NoResultTakesParamsAndBody, HasResult, HasResultTakesParams,
               HasResultTakesBody, HasResultTakesParamsAndBody>
      handler_{};
};

/**
 * @brief Data used by server to verify and handle request.
 */
struct Endpoint {
  EndpointDesc desc{};
  EndpointHandler handler;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_ENDPOINT_H_
