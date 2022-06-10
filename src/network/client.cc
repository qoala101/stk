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
    const std::map<std::string, json::TypedAny> &params,
    const std::map<std::string, json::TypeVariant> &endpoint_params,
    network::RestRequest &request) {
  for (const auto &endpoint_param : endpoint_params) {
    const auto &[param_name, param_type] = endpoint_param;
    const auto param = params.find(param_name);

    if (const auto no_param = param == params.end()) {
      ABSL_ASSERT(param_type.IsOptional() &&
                  "Request misses mandatory parameter");

      continue;
    }

    const auto &value = param->second;
    const auto value_json = param_type.ConvertAnyToJson(value.GetAny());

    const auto not_converted = !value_json.has_value();
    ABSL_ASSERT(!not_converted && "Parameter {} has wrong type");

    if (const auto no_value_for_optional_param =
            value_json->is_null() && param_type.IsOptional()) {
      continue;
    }

    request.AddParameter(param_name, value_json->serialize());
  }
}

[[nodiscard]] auto ConvertRequestBodyToJson(
    const json::TypedAny &request_body,
    const std::optional<json::TypeVariant> &endpoint_request_body)
    -> web::json::value {
  if (!endpoint_request_body.has_value()) {
    return web::json::value{};
  }

  const auto no_mandatory_body =
      !request_body.GetAny().HasValue() && !endpoint_request_body->IsOptional();
  ABSL_ASSERT(!no_mandatory_body && "Request misses mandatory body");

  auto json = endpoint_request_body->ConvertAnyToJson(request_body.GetAny());

  const auto not_converted = !json.has_value();
  ABSL_ASSERT(!not_converted && "Request has wrong body type");

  return std::move(*json);
}

/**
 * @throws If response misses mandatory body or body represents an exception.
 */
[[nodiscard]] auto ParseResponseBody(
    const web::json::value &response_json,
    const std::optional<json::TypeVariant> &endpoint_response_body) -> Result {
  if (response_json.is_null()) {
    if (const auto no_mandatory_result =
            endpoint_response_body.has_value() &&
            !endpoint_response_body->IsOptional()) {
      Logger().error("Response misses mandatory body");
      throw std::runtime_error{"Response misses mandatory body"};
    }

    return endpoint_response_body->MakeNulloptAny();
  }

  if (endpoint_response_body.has_value()) {
    auto response_body =
        endpoint_response_body->ParseAnyFromJson(response_json);

    if (const auto response_body_parsed = response_body.HasValue()) {
      return response_body;
    }
  }

  const auto exception =
      json::Type<std::runtime_error>{}.ParseFromJson(response_json);

  if (const auto exception_parsed = exception.has_value()) {
    Logger().error("Received exception {}", exception->what());
    throw std::runtime_error{*exception};
  }

  Logger().error("Received wrong response type {}", response_json.serialize());
  throw std::runtime_error{"Received wrong response type"};
}
}  // namespace

class Client::Impl {
 public:
  explicit Impl(const Uri &uri) : base_uri_{uri.GetFullUri()} {}

  [[nodiscard]] auto Execute(const EndpointDesc &endpoint) const -> Result {
    return Execute(endpoint, {}, {});
  }

  [[nodiscard]] auto Execute(
      const EndpointDesc &endpoint,
      const std::map<std::string, json::TypedAny> &params) const -> Result {
    return Execute(endpoint, params, {});
  }

  [[nodiscard]] auto Execute(const EndpointDesc &endpoint,
                             const json::TypedAny &request_body) const
      -> Result {
    return Execute(endpoint, {}, request_body);
  }

  [[nodiscard]] auto Execute(
      const EndpointDesc &endpoint,
      const std::map<std::string, json::TypedAny> &params,
      const json::TypedAny &request_body) const -> Result {
    auto request = network::RestRequest{endpoint.method, base_uri_}.AppendUri(
        endpoint.relative_uri);

    AddParamsToRequest(params, endpoint.params, request);

    auto request_json =
        ConvertRequestBodyToJson(request_body, endpoint.request_body);

    if (!request_json.is_null()) {
      request.SetJson(request_json);
    }

    Logger().info("Sending request {} {}", endpoint.method,
                  endpoint.relative_uri);

    const auto response_json = request.SendAndGetResponse();

    if (const auto request_failed = !response_json.has_value()) {
      Logger().error("Request failed {} {}", endpoint.method,
                     endpoint.relative_uri);
      throw std::runtime_error{"Request failed"};
    }

    return ParseResponseBody(*response_json, endpoint.response_body);
  }

 private:
  const std::string base_uri_{};
};

Client::Client(const Uri &uri) : impl_{std::make_unique<Impl>(uri)} {}

Client::~Client() = default;

auto Client::Execute(const EndpointDesc &endpoint) const -> Result {
  return impl_->Execute(endpoint);
}

auto Client::Execute(const EndpointDesc &endpoint,
                     const std::map<std::string, json::TypedAny> &params) const
    -> Result {
  return impl_->Execute(endpoint, params);
}

auto Client::Execute(const EndpointDesc &endpoint,
                     const json::TypedAny &request_body) const -> Result {
  return impl_->Execute(endpoint, request_body);
}

auto Client::Execute(const EndpointDesc &endpoint,
                     const std::map<std::string, json::TypedAny> &params,
                     const json::TypedAny &request_body) const -> Result {
  return impl_->Execute(endpoint, params, request_body);
}
}  // namespace stonks::network