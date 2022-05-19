#include "server.h"

#include <absl/base/macros.h>
#include <cpprest/http_listener.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <exception>
#include <gsl/assert>
#include <range/v3/algorithm/find_if.hpp>

namespace stonks::network {
namespace {
auto Logger() -> spdlog::logger & {
  static auto logger = []() {
    auto logger = spdlog::stdout_color_mt("Server");
    logger->set_level(spdlog::level::debug);
    return logger;
  }();

  return *logger;
}
}  // namespace

struct RequestHandlerResult {
  web::http::status_code status_code{};
  web::json::value response_body{};
} __attribute__((aligned(16)));  // NOLINT(*-magic-numbers)

class Server::Impl {
 public:
  explicit Impl(std::string_view base_uri, std::vector<Endpoint> endpoints)
      : endpoints_{std::move(endpoints)},
        http_listener_{std::string{base_uri}} {
    Expects(!base_uri.empty());
    Expects(!endpoints_.empty());

    http_listener_.support([this](const web::http::http_request &request) {
      const auto result = RequestHandler(request);

      if (result.response_body.is_null()) {
        Logger().debug("Replying {} without body", result.status_code);
        request.reply(result.status_code);
        return;
      }

      Logger().debug("Replying {} with body", result.status_code);
      request.reply(result.status_code, result.response_body);
    });
    http_listener_.open();
  }

  Impl(Impl &&) = default;
  auto operator=(Impl &&) -> Impl & = default;

  Impl(const Impl &) = delete;
  auto operator=(const Impl &) -> Impl & = delete;

  ~Impl() { http_listener_.close().wait(); }

  /**
   * @throws If endpoint is not registered.
   */
  [[nodiscard]] auto FindEndpoint(const web::http::http_request &request) const
      -> const Endpoint & {
    const auto endpoint = ranges::find_if(
        endpoints_, [&request_method = request.method(),
                     &request_relative_uri = request.relative_uri().path()](
                        const Endpoint &endpoint) {
          return (endpoint.desc.method == request_method) &&
                 (endpoint.desc.relative_uri == request_relative_uri);
        });

    if (const auto no_such_endpoint = endpoint == endpoints_.end()) {
      throw std::runtime_error{"Unknown endpoint " +
                               request.request_uri().path()};
    }

    return *endpoint;
  }

  /**
   * @throws If parameters are wrong.
   */
  [[nodiscard]] static auto ParseParams(
      const std::map<std::string, std::string> &raw_params,
      const std::map<std::string, json::AnyDesc> &endpoint_params)
      -> std::map<std::string, json::Any> {
    auto parsed_params = std::map<std::string, json::Any>{};

    for (const auto &endpoint_param : endpoint_params) {
      const auto &[param_name, param_desc] = endpoint_param;
      const auto raw_param = raw_params.find(param_name);

      if (const auto no_param = raw_param == raw_params.end()) {
        if (const auto param_is_mandatory =
                param_desc.optional == json::OptionalFlag::kMandatory) {
          throw std::runtime_error{"Request misses mandatory parameter " +
                                   param_name};
        }

        continue;
      }

      const auto &raw_value = raw_param->second;
      auto parsed_value = std::visit(
          [&raw_value](const auto &variant) {
            try {
              return variant.ParseAnyFromJson(
                  web::json::value::parse(raw_value));
            } catch (const std::exception &) {
            }

            return std::any{};
          },
          param_desc.converter);

      if (const auto not_parsed = !parsed_value.has_value()) {
        throw std::runtime_error{"Cannot parse parameter " + param_name};
      }

      parsed_params[param_name] = json::Any{.value = std::move(parsed_value),
                                            .converter = param_desc.converter};
    }

    return parsed_params;
  }

  /**
   * @throws If request body is wrong.
   */
  [[nodiscard]] static auto ParseRequestBody(
      const web::http::http_request &request,
      const std::optional<json::AnyDesc> &endpoint_request_body) -> json::Any {
    if (!endpoint_request_body.has_value()) {
      return json::Any{};
    }

    const auto json = request.extract_json().get();
    auto request_body = json::Any{};

    request_body.value = std::visit(
        [&json](const auto &variant) {
          try {
            return variant.ParseAnyFromJson(json);
          } catch (const std::exception &) {
          }

          return std::any{};
        },
        endpoint_request_body->converter);

    if (const auto missing_mandatory_object =
            !request_body.value.has_value() &&
            (endpoint_request_body->optional ==
             json::OptionalFlag::kMandatory)) {
      throw std::runtime_error{"Request misses mandatory body " +
                               json.serialize()};
    }

    request_body.converter = endpoint_request_body->converter;
    return request_body;
  }

  [[nodiscard]] static auto ConvertResponseBodyToJson(
      const std::any &response_body,
      const std::optional<json::AnyDesc> &endpoint_response_body)
      -> web::json::value {
    if (!endpoint_response_body.has_value()) {
      return web::json::value{};
    }

    const auto no_mandatory_result =
        !response_body.has_value() &&
        (endpoint_response_body->optional == json::OptionalFlag::kMandatory);
    ABSL_ASSERT(!no_mandatory_result && "Response misses mandatory body");

    auto json = std::visit(
        [&response_body](
            const auto &variant) -> std::optional<web::json::value> {
          try {
            return variant.ConvertAnyToJson(response_body);
          } catch (const std::exception &) {
          }

          return std::nullopt;
        },
        endpoint_response_body->converter);

    const auto not_converted = !json.has_value();
    ABSL_ASSERT(!not_converted && "Response body has wrong type");

    return *json;
  }

  [[nodiscard]] auto RequestHandler(
      const web::http::http_request &request) const -> RequestHandlerResult {
    Logger().debug("Got {} request on {}", request.method(),
                   request.relative_uri().path());

    const auto *endpoint = (const Endpoint *){};
    auto parsed_params = std::map<std::string, json::Any>{};
    auto parsed_request_body = json::Any{};

    try {
      endpoint = &FindEndpoint(request);

      const auto raw_params =
          web::uri::split_query(request.request_uri().query());
      parsed_params = ParseParams(raw_params, endpoint->desc.params);

      parsed_request_body =
          ParseRequestBody(request, endpoint->desc.request_body);
    } catch (const std::exception &exception) {
      Logger().error("Bad request {}", exception.what());
      return RequestHandlerResult{
          .status_code = web::http::status_codes::BadRequest,
          .response_body =
              json::ConvertToJson(std::runtime_error{exception.what()})};
    }

    auto response_body = std::any{};

    try {
      response_body = endpoint->handler(std::move(parsed_params),
                                        std::move(parsed_request_body));
    } catch (const std::exception &exception) {
      Logger().error("Request handler thrown exception {}", exception.what());
      return RequestHandlerResult{
          .status_code = web::http::status_codes::InternalError,
          .response_body =
              json::ConvertToJson(std::runtime_error{exception.what()})};
    }

    auto response_json =
        ConvertResponseBodyToJson(response_body, endpoint->desc.response_body);
    return RequestHandlerResult{.status_code = web::http::status_codes::OK,
                                .response_body = std::move(response_json)};
  }

 private:
  web::http::experimental::listener::http_listener http_listener_{};
  std::vector<Endpoint> endpoints_{};
};

Server::~Server() = default;

Server::Server(std::string_view base_uri, std::vector<Endpoint> endpoints)
    : impl_{std::make_unique<Impl>(base_uri, std::move(endpoints))} {
  Expects(!base_uri.empty());
}
}  // namespace stonks::network