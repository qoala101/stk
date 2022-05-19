#include "server.h"

#include <cpprest/http_listener.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <gsl/assert>
#include <range/v3/algorithm/find_if.hpp>

namespace stonks {
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

  [[nodiscard]] auto FindEndpoint(const web::http::http_request &request) const
      -> const Endpoint * {
    const auto endpoint = ranges::find_if(
        endpoints_, [&request_method = request.method(),
                     &request_relative_uri = request.relative_uri().path()](
                        const Endpoint &endpoint) {
          return (endpoint.desc.method == request_method) &&
                 (endpoint.desc.relative_uri == request_relative_uri);
        });

    if (const auto no_such_endpoint = endpoint == endpoints_.end()) {
      Logger().error("Unknown endpoint {}", request.request_uri().path());
      return nullptr;
    }

    return endpoint.base();
  }

  [[nodiscard]] static auto ParseParams(
      const std::map<std::string, std::string> &raw_params,
      const std::map<std::string, json::AnyDesc> &endpoint_params)
      -> std::optional<std::map<std::string, json::Any>> {
    auto parsed_params = std::map<std::string, json::Any>{};

    for (const auto &endpoint_param : endpoint_params) {
      const auto &[param_name, param_desc] = endpoint_param;
      const auto raw_param = raw_params.find(param_name);

      if (const auto no_param = raw_param == raw_params.end()) {
        if (const auto param_is_mandatory =
                param_desc.optional == json::OptionalFlag::kMandatory) {
          Logger().error("Request misses mandatory parameter {}", param_name);
          return std::nullopt;
        }

        continue;
      }

      const auto &raw_value = raw_param->second;
      auto parsed_value = std::visit(
          [&raw_value](const auto &variant) -> std::optional<std::any> {
            // TODO(vh): make throw here?
            const auto parsed_value =
                variant.ParseFromJson(web::json::value::parse(raw_value));

            if (parsed_value.has_value()) {
              // TODO(vh): move here?
              return *parsed_value;
            }

            return std::nullopt;
          },
          param_desc.converter);

      if (const auto not_parsed = !parsed_value->has_value()) {
        Logger().error("Cannot parse parameter {}", param_name);
        return std::nullopt;
      }

      parsed_params[param_name] = json::Any{.value = std::move(*parsed_value),
                                            .converter = param_desc.converter};
    }

    // TODO(vh): would this make a copy when constructing optional?
    return parsed_params;
  }

  [[nodiscard]] static auto ParseRequestBody(
      const web::http::http_request &request,
      const std::optional<json::AnyDesc> &endpoint_request_body)
      -> std::optional<json::Any> {
    if (!endpoint_request_body.has_value()) {
      return json::Any{};
    }

    const auto json = request.extract_json().get();
    auto request_body = json::Any{};

    request_body.value = std::visit(
        [&json](const auto &variant) -> std::any {
          auto object = variant.ParseFromJson(json);

          if (object.has_value()) {
            return std::move(*object);
          }

          return {};
        },
        endpoint_request_body->converter);

    if (const auto missing_mandatory_object =
            !request_body.value.has_value() &&
            (endpoint_request_body->optional ==
             json::OptionalFlag::kMandatory)) {
      Logger().error("Request misses mandatory body {}", json.serialize());
      return std::nullopt;
    }

    request_body.converter = endpoint_request_body->converter;
    return request_body;
  }

  [[nodiscard]] static auto ConvertResponseBodyToJson(
      const std::any &response_body,
      const std::optional<json::AnyDesc> &endpoint_response_body)
      -> std::optional<web::json::value> {
    if (!endpoint_response_body.has_value()) {
      return web::json::value{};
    }

    if (const auto no_mandatory_result =
            !response_body.has_value() && (endpoint_response_body->optional ==
                                           json::OptionalFlag::kMandatory)) {
      Logger().error("Response misses mandatory body");
      return std::nullopt;
    }

    auto json = std::visit(
        [&response_body](
            const auto &variant) -> std::optional<web::json::value> {
          try {
            return variant.any_ConvertToJson(response_body);
          } catch (const std::exception &exception) {
            return std::nullopt;
          }
        },
        endpoint_response_body->converter);

    if (const auto not_converted = !json.has_value()) {
      Logger().error("Response body has wrong type");
      return std::nullopt;
    }

    // TODO(vh): should I add std::move here?
    return *json;
  }

  [[nodiscard]] auto RequestHandler(
      const web::http::http_request &request) const -> RequestHandlerResult {
    Logger().debug("Got {} request on {}", request.method(),
                   request.relative_uri().path());

    const auto *const endpoint = FindEndpoint(request);

    if (endpoint == nullptr) {
      return RequestHandlerResult{.status_code =
                                      web::http::status_codes::NotFound};
    }

    const auto raw_params =
        web::uri::split_query(request.request_uri().query());
    auto parsed_params = ParseParams(raw_params, endpoint->desc.params);

    if (!parsed_params.has_value()) {
      return RequestHandlerResult{.status_code =
                                      web::http::status_codes::BadRequest};
    }

    auto parsed_request_body =
        ParseRequestBody(request, endpoint->desc.request_body);

    if (!parsed_request_body.has_value()) {
      return RequestHandlerResult{.status_code =
                                      web::http::status_codes::BadRequest};
    }

    auto response_body = std::any{};

    try {
      response_body = endpoint->handler(std::move(*parsed_params),
                                        std::move(*parsed_request_body));
    } catch (const std::exception &exception) {
      // TODO(vh): return exception what to the client and handle it in client
    }

    auto response_json =
        ConvertResponseBodyToJson(response_body, endpoint->desc.response_body);

    if (!response_json.has_value()) {
      return RequestHandlerResult{.status_code =
                                      web::http::status_codes::InternalError};
    }

    return RequestHandlerResult{.status_code = web::http::status_codes::OK,
                                .response_body = *response_json};
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
}  // namespace stonks