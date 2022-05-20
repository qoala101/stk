#include "client.h"

#include <absl/base/macros.h>
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <exception>
#include <gsl/assert>
#include <stdexcept>

#include "json_conversions.h"
#include "rest_request.h"

namespace stonks::network {
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

  static void AddParamsToRequest(
      const std::map<std::string, json::Any> &params,
      const std::map<std::string, json::AnyDesc> &endpoint_params,
      network::RestRequest &request) {
    for (const auto &endpoint_param : endpoint_params) {
      const auto &[param_name, param_desc] = endpoint_param;
      const auto param = params.find(param_name);

      if (const auto no_param = param == params.end()) {
        const auto param_is_mandatory =
            param_desc.optional == json::OptionalFlag::kMandatory;
        ABSL_ASSERT(!param_is_mandatory &&
                    "Request misses mandatory parameter");

        continue;
      }

      const auto &value = param->second;
      auto value_json = std::visit(
          [&value](const auto &variant) -> std::optional<web::json::value> {
            try {
              return variant.ConvertAnyToJson(value.value);
            } catch (const std::exception &) {
            }

            return std::nullopt;
          },
          param_desc.converter);

      const auto not_converted = !value_json.has_value();
      ABSL_ASSERT(!not_converted && "Parameter {} has wrong type");

      if (const auto no_value_for_optional_param =
              value_json->is_null() &&
              (param_desc.optional == json::OptionalFlag::kOptional)) {
        continue;
      }

      request.AddParameter(param_name, value_json->serialize());
    }
  }

  [[nodiscard]] static auto ConvertRequestBodyToJson(
      const json::Any &request_body,
      const std::optional<json::AnyDesc> &endpoint_request_body)
      -> web::json::value {
    if (!endpoint_request_body.has_value()) {
      return web::json::value{};
    }

    const auto no_mandatory_body =
        !request_body.value.has_value() &&
        (endpoint_request_body->optional == json::OptionalFlag::kMandatory);
    ABSL_ASSERT(!no_mandatory_body && "Request misses mandatory body");

    auto json = std::visit(
        [&value = request_body.value](
            const auto &variant) -> std::optional<web::json::value> {
          try {
            return variant.ConvertAnyToJson(value);
          } catch (const std::exception &) {
          }

          return std::nullopt;
        },
        endpoint_request_body->converter);

    const auto not_converted = !json.has_value();
    ABSL_ASSERT(!not_converted && "Request has wrong body type");

    // TODO(vh): should I add std::move here?
    return *json;
  }

  /**
   * @throws If response misses mandatory body or body represents an exception.
   */
  [[nodiscard]] static auto ParseResponseBody(
      const web::json::value &response_json,
      const std::optional<json::AnyDesc> &endpoint_response_body) -> std::any {
    if (response_json.is_null()) {
      if (const auto no_mandatory_result = endpoint_response_body.has_value() &&
                                           (endpoint_response_body->optional ==
                                            json::OptionalFlag::kMandatory)) {
        Logger().error("Response misses mandatory body");
        throw std::runtime_error{"Response misses mandatory body"};
      }

      return std::visit(
          [](const auto &variant) { return variant.MakeNulloptAny(); },
          endpoint_response_body->converter);
    }

    if (endpoint_response_body.has_value()) {
      auto response_body = std::visit(
          [&response_json](const auto &variant) {
            try {
              return variant.ParseAnyFromJson(response_json);
            } catch (const std::exception &) {
            }

            return std::any{};
          },
          endpoint_response_body->converter);

      if (const auto response_body_parsed = response_body.has_value()) {
        return response_body;
      }
    }

    const auto exception =
        json::ParseFromJson<std::runtime_error>(response_json);

    if (const auto exception_parsed = exception.has_value()) {
      Logger().error("Received exception {}", exception->what());
      throw std::runtime_error{*exception};
    }

    Logger().error("Received wrong response type {}",
                   response_json.serialize());
    throw std::runtime_error{"Received wrong response type"};
  }

  /**
   * @throws If response doesn't match endpoint description or response is an
   * exception itself.
   */
  [[nodiscard]] auto Execute(const EndpointDesc &endpoint,
                             const std::map<std::string, json::Any> &params,
                             const json::Any &request_body) const -> std::any {
    auto request = network::RestRequest{endpoint.method, base_uri_}.AppendUri(
        endpoint.relative_uri);

    AddParamsToRequest(params, endpoint.params, request);

    auto request_json =
        ConvertRequestBodyToJson(request_body, endpoint.request_body);

    if (!request_json.is_null()) {
      request.SetJson(request_json);
    }

    Logger().debug("Sending request {} {}", endpoint.method,
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
}  // namespace stonks::network