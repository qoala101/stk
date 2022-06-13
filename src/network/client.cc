#include "client.h"

#include <absl/base/macros.h>
#include <bits/exception.h>
#include <cpprest/details/basic_types.h>
#include <cpprest/json.h>
#include <fmt/format.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "any.h"
#include "endpoint.h"
#include "rest_request.h"
#include "type.h"
#include "type_variant.h"

namespace stonks::network {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger = spdlog::stdout_color_mt("Client");
  return *logger;
}

void AddParamsToRequest(
    const params &params,
    const std::map<std::string, Optional> &endpoint_params,
    RestRequest &request) {
  for (const auto &endpoint_param : endpoint_params) {
    const auto &[param_name, param_type] = endpoint_param;
    const auto param = params.find(param_name);

    if (const auto no_param = param == params.end()) {
      ABSL_ASSERT((param_type == Optional::kOptional) &&
                  "Request misses mandatory parameter");

      continue;
    }

    const auto &value = param->second;
    const auto value_json = value.GetJson();

    if (const auto no_value_for_optional_param =
            value_json.is_null() &&
            (param_type == Optional::kOptional)) {
      continue;
    }

    request.AddParameter(param_name, value_json.serialize());
  }
}

void AddBodyToRequest(const v2_Body &request_body,
                      Optional endpoint_request_body,
                      RestRequest &request) {
  const auto &request_json = request_body.GetJson();

  if (!request_json.is_null()) {
    request.SetJson(request_json);
    return;
  }

  ABSL_ASSERT((endpoint_request_body == Optional::kOptional) &&
              "Request misses mandatory body");
}

auto SendRequest(const EndpointDesc &endpoint, RestRequest &request)
    -> web::json::value {
  Logger().info("Sending {} request to {}", endpoint.method,
                endpoint.relative_uri);

  const auto response_json = request.SendAndGetResponse();

  if (const auto request_failed = !response_json.has_value()) {
    Logger().error("{} request to {} failed", endpoint.method,
                   endpoint.relative_uri);
    throw std::runtime_error{"Request failed"};
  }

  return *response_json;
}

/**
 * @throws If response misses mandatory body or body represents an exception.
 */
[[nodiscard]] auto ParseResponseBody(
    const web::json::value &response_json,
    Optional endpoint_request_body) -> v2_Result {
  if (const auto no_mandatory_result =
          response_json.is_null() &&
          (endpoint_request_body == Optional::kMandatory)) {
    Logger().error("Response misses mandatory body");
    throw std::runtime_error{"Response misses mandatory body"};
  }

  return response_json;
}
}  // namespace

Client::Client(const Uri &uri) : base_uri_{uri.GetFullUri()} {}

auto Client::Execute(const EndpointDesc &endpoint) const -> v2_Result {
  return Execute(endpoint, {}, {{}});
}

auto Client::Execute(const EndpointDesc &endpoint,
                     const params &params) const -> v2_Result {
  return Execute(endpoint, params, {{}});
}

auto Client::Execute(const EndpointDesc &endpoint,
                     const v2_Body &request_body) const -> v2_Result {
  return Execute(endpoint, {}, request_body);
}

auto Client::Execute(const EndpointDesc &endpoint, const params &params,
                     const v2_Body &request_body) const -> v2_Result {
  auto request =
      RestRequest{endpoint.method, base_uri_}.AppendUri(endpoint.relative_uri);

  AddParamsToRequest(params, endpoint.params, request);
  AddBodyToRequest(request_body, endpoint.request_body, request);
  auto response_json = SendRequest(endpoint, request);
  return ParseResponseBody(response_json, endpoint.response_body);
}
}  // namespace stonks::network