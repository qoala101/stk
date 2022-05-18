#include "server.h"

#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <cmath>
#include <exception>
#include <range/v3/algorithm/find_if.hpp>
#include <utility>

#include "client_server_types.h"
#include "json_conversions.h"

namespace stonks {
namespace {
auto Logger() -> spdlog::logger & {
  static auto logger = spdlog::stdout_color_mt("Server");
  return *logger;
}
}  // namespace

class Server::Impl {
 public:
  explicit Impl(std::string_view base_uri, std::vector<EndpointDesc> endpoints)
      : endpoints_{std::move(endpoints)},
        http_listener_{std::string{base_uri}} {
    http_listener_.support([this](const web::http::http_request &request) {
      const auto &[result_code, json] = RequestHandler(request);

      if (json.has_value()) {
        spdlog::info("Replying: {}", json->serialize());
        request.reply(result_code, *json);
        return;
      }

      spdlog::info("Replying nothing");
      request.reply(result_code);
    });
    http_listener_.open();
  }

  ~Impl() { http_listener_.close().wait(); }

  [[nodiscard]] auto RequestHandler(const web::http::http_request &request)
      -> std::pair<web::http::status_code, std::optional<web::json::value>> {
    Logger().info("Got {} request on {}", request.method(),
                  request.relative_uri().path());
    const auto endpoint = ranges::find_if(
        endpoints_, [&request_method = request.method(),
                     &request_relative_uri = request.relative_uri().path()](
                        const EndpointDesc &endpoint) {
          return (endpoint.method == request_method) &&
                 (endpoint.relative_uri == request_relative_uri);
        });

    if (const auto no_such_endpoint = endpoint == endpoints_.end()) {
      Logger().error("Unknown endpoint {}", request.request_uri().path());
      return {web::http::status_codes::NotFound, std::nullopt};
    }

    const auto raw_params =
        web::uri::split_query(request.request_uri().query());
    auto parsed_params = std::map<std::string, json::Any>{};

    for (const auto &endpoint_param : endpoint->params) {
      const auto &param_name = endpoint_param.first;
      const auto &param_desc = endpoint_param.second;
      const auto raw_param = raw_params.find(param_name);

      if (const auto no_param = raw_param == raw_params.end()) {
        if (const auto param_is_mandatory =
                param_desc.optional == json::OptionalFlag::kMandatory) {
          Logger().error("Request misses mandatory parameter {}", param_name);
          return {web::http::status_codes::BadRequest, std::nullopt};
        }

        continue;
      }

      const auto &raw_value = raw_param->second;
      const auto value_parsed = std::visit(
          [&param_name, &converter = param_desc.converter, &raw_value,
           &parsed_params](const auto &variant) {
            auto value =
                variant.ParseFromJson(web::json::value::parse(raw_value));

            if (value.has_value()) {
              parsed_params[param_name] =
                  json::Any{.value = std::move(*value), .converter = converter};
              return true;
            }

            return false;
          },
          param_desc.converter);

      if (!value_parsed) {
        Logger().error("Cannot parse parameter {}", param_name);
        return {web::http::status_codes::BadRequest, std::nullopt};
      }
    }

    auto parsed_object = json::Any{};

    if (endpoint->object.has_value()) {
      const auto json = request.extract_json().get();

      parsed_object.value = std::visit(
          [&json](const auto &variant) -> std::any {
            auto object = variant.ParseFromJson(json);

            if (object.has_value()) {
              return std::move(*object);
            }

            return {};
          },
          endpoint->object->converter);

      if (const auto missing_mandatory_object =
              !parsed_object.value.has_value() &&
              (endpoint->object->optional == json::OptionalFlag::kMandatory)) {
        Logger().error("Request misses mandatory object {}", json.serialize());
        return {web::http::status_codes::BadRequest, std::nullopt};
      }

      parsed_object.converter = endpoint->object->converter;
    }

    auto result = std::any{};

    try {
      result = endpoint->handler_(std::move(parsed_params),
                                  std::move(parsed_object));
    } catch (const std::exception &exception) {
      // TODO(vh): return exception what to the client and handle it in client
    }

    auto result_json = std::optional<web::json::value>{std::nullopt};

    if (endpoint->result.has_value()) {
      if (const auto no_mandatory_result =
              !result.has_value() &&
              (endpoint->result->optional == json::OptionalFlag::kMandatory)) {
        Logger().error("Response misses mandatory result");
        return {web::http::status_codes::InternalError, std::nullopt};
      }

      result_json = std::visit(
          [&result](const auto &variant) -> std::optional<web::json::value> {
            try {
              return variant.any_ConvertToJson(result);
            } catch (const std::exception &exception) {
              return std::nullopt;
            }
          },
          endpoint->result->converter);

      if (const auto not_converted = !result_json.has_value()) {
        Logger().error("Response has wrong result type");
        return {web::http::status_codes::InternalError, std::nullopt};
      }
    }

    return {web::http::status_codes::OK, result_json};
  }

 private:
  web::http::experimental::listener::http_listener http_listener_{};
  const std::vector<EndpointDesc> endpoints_{};
};

Server::~Server() = default;

Server::Server(std::string_view base_uri, std::vector<EndpointDesc> endpoints)
    : impl_{std::make_unique<Impl>(base_uri, std::move(endpoints))} {}
}  // namespace stonks