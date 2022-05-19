#include "client.h"

#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <gsl/assert>

#include "rest_request.h"

namespace stonks {
namespace {
auto Logger() -> spdlog::logger & {
  static auto logger = []() {
    auto logger = spdlog::stdout_color_mt("Client");
    logger->set_level(spdlog::level::debug);
    return logger;
  }();

  return *logger;
}
}  // namespace

class Client::Impl {
 public:
  explicit Impl(std::string_view base_uri) : base_uri_{base_uri} {
    Expects(!base_uri.empty());
  }

  [[nodiscard]] static auto AddParamsToRequest(
      const std::map<std::string, json::Any> &params,
      const std::map<std::string, json::AnyDesc> &endpoint_params,
      rest::RestRequest &request) -> bool {
    for (const auto &endpoint_param : endpoint_params) {
      const auto &[param_name, param_desc] = endpoint_param;
      const auto param = params.find(param_name);

      if (const auto no_param = param == params.end()) {
        if (const auto param_is_mandatory =
                param_desc.optional == json::OptionalFlag::kMandatory) {
          Logger().error("Request misses mandatory parameter {}", param_name);
          return false;
        }

        continue;
      }

      const auto &value = param->second;
      auto value_json = std::visit(
          [&value](const auto &variant) -> std::optional<web::json::value> {
            try {
              return variant.any_ConvertToJson(value.value);
            } catch (const std::exception &exception) {
              return std::nullopt;
            }
          },
          param_desc.converter);

      if (const auto not_converted = !value_json.has_value()) {
        Logger().error("Parameter {} has wrong type", param_name);
        return false;
      }

      request.AddParameter(param_name, value_json->serialize());
    }

    return true;
  }

  [[nodiscard]] static auto ConvertRequestBodyToJson(
      const json::Any &request_body,
      const std::optional<json::AnyDesc> &endpoint_request_body)
      -> std::optional<web::json::value> {
    if (!endpoint_request_body.has_value()) {
      return web::json::value{};
    }

    if (const auto no_mandatory_object = !request_body.value.has_value() &&
                                         (endpoint_request_body->optional ==
                                          json::OptionalFlag::kMandatory)) {
      // TODO(vh): maybe throw?
      Logger().error("Request misses mandatory body");
      return std::nullopt;
    }

    auto json = std::visit(
        [&value = request_body.value](
            const auto &variant) -> std::optional<web::json::value> {
          try {
            return variant.any_ConvertToJson(value);
          } catch (...) {
            return std::nullopt;
          }
        },
        request_body.converter);

    if (const auto not_converted = !json.has_value()) {
      Logger().error("Request has wrong body type");
      return std::nullopt;
    }

    // TODO(vh): should I add std::move here?
    return *json;
  }

  [[nodiscard]] static auto ParseResponseBody(
      const std::optional<web::json::value> &response_json,
      const std::optional<json::AnyDesc> &endpoint_response_body)
      -> std::optional<std::any> {
    if (!endpoint_response_body.has_value()) {
      return std::any{};
    }

    if (!response_json.has_value()) {
      if (const auto no_mandatory_result = endpoint_response_body->optional ==
                                           json::OptionalFlag::kMandatory) {
        // TODO(vh): maybe throw?
        Logger().error("Response misses mandatory body");
        return std::nullopt;
      }

      return std::any{};
    }

    const auto response_body = std::visit(
        [&response_json](const auto &variant) -> std::any {
          return variant.any_ParseFromJson(*response_json);
        },
        endpoint_response_body->converter);

    if (const auto not_parsed = !response_body.has_value()) {
      Logger().error("Response has wrong body type");
      return std::nullopt;
    }

    // TODO(vh): is move constructor called when returning const?
    return response_body;
  }

  [[nodiscard]] auto Execute(const EndpointDesc &endpoint,
                             const std::map<std::string, json::Any> &params,
                             const json::Any &request_body) const -> std::any {
    auto request = rest::RestRequest{endpoint.method, base_uri_}.AppendUri(
        endpoint.relative_uri);

    const auto params_successfully_added =
        AddParamsToRequest(params, endpoint.params, request);

    if (!params_successfully_added) {
      return {};
    }

    auto request_json =
        ConvertRequestBodyToJson(request_body, endpoint.request_body);

    if (const auto bad_request_body = !request_json.has_value()) {
      return {};
    }

    if (!request_json->is_null()) {
      request.SetJson(*request_json);
    }

    Logger().debug("Sending {} request to {}", endpoint.method,
                   endpoint.relative_uri);

    const auto response_json = request.SendAndGetResponse();
    const auto response_body =
        ParseResponseBody(response_json, endpoint.response_body);

    if (const auto bad_response_body = !response_body.has_value()) {
      return {};
    }

    // TODO(vh): nothrow here but throw in errors
    return *response_body;
  }

 private:
  const std::string base_uri_{};
};

Client::~Client() = default;

Client::Client(std::string_view base_uri)
    : impl_{std::make_unique<Impl>(base_uri)} {
  Expects(!base_uri.empty());
}

auto Client::Execute(const EndpointDesc &endpoint,
                     const std::map<std::string, json::Any> &params,
                     const json::Any &request_body) const -> std::any {
  return impl_->Execute(endpoint, params, request_body);
}
}  // namespace stonks