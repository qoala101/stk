#include "server.h"

#include <absl/base/macros.h>
#include <bits/exception.h>
#include <cpprest/base_uri.h>
#include <cpprest/details/basic_types.h>
#include <cpprest/http_listener.h>
#include <cpprest/http_msg.h>
#include <cpprest/json.h>
#include <fmt/format.h>
#include <pplx/pplxtasks.h>
#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <map>
#include <optional>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/functional/identity.hpp>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include "any.h"
#include "any_map.h"
#include "endpoint.h"
#include "json_conversions.h"
#include "type_variant.h"

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
};

class Server::Impl {
 public:
  explicit Impl(const LocalUri &uri, std::vector<Endpoint> endpoints)
      : endpoints_{std::move(endpoints)}, http_listener_{uri.GetFullUri()} {
    PrependBaseUriToEndpoints();

    http_listener_.support([this](const web::http::http_request &request) {
      const auto result = RequestHandler(request);
      auto response = web::http::http_response{result.status_code};

      if (!result.response_body.is_null()) {
        response.set_body(result.response_body);
      }

      Logger().debug("Replying {}", result.status_code);
      request.reply(response);
    });
    http_listener_.open();
  }

  Impl(const Impl &) = delete;
  auto operator=(const Impl &) -> Impl & = delete;

  Impl(Impl &&) noexcept = default;
  auto operator=(Impl &&) noexcept -> Impl & = default;

  ~Impl() { http_listener_.close().wait(); }

  void PrependBaseUriToEndpoints() {
    const auto &base_uri = http_listener_.uri().path();

    for (auto &endpoint : endpoints_) {
      endpoint.desc.relative_uri.insert(0, base_uri);
    }
  }

  /**
   * @throws If endpoint is not registered.
   */
  [[nodiscard]] auto FindEndpoint(const web::http::http_request &request) const
      -> const Endpoint & {
    const auto endpoint = ranges::find_if(
        endpoints_, [&request_method = request.method(),
                     &request_relative_uri = request.request_uri().path()](
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
      const std::map<std::string, json::TypeVariant> &endpoint_params)
      -> Params {
    auto parsed_params = Params{};

    for (const auto &endpoint_param : endpoint_params) {
      const auto &[param_name, param_type] = endpoint_param;
      const auto raw_param = raw_params.find(param_name);

      if (const auto no_param = raw_param == raw_params.end()) {
        if (!param_type.IsOptional()) {
          throw std::runtime_error{"Request misses mandatory parameter " +
                                   param_name};
        }

        parsed_params.Set(param_name, param_type.MakeNulloptAny());

        continue;
      }

      const auto &raw_value = raw_param->second;
      auto parsed_value =
          param_type.ParseAnyFromJson(web::json::value::parse(raw_value));

      if (const auto not_parsed = !parsed_value.HasValue()) {
        throw std::runtime_error{"Cannot parse parameter " + param_name};
      }

      parsed_params.Set(param_name, std::move(parsed_value));
    }

    return parsed_params;
  }

  /**
   * @throws If request body is wrong.
   */
  [[nodiscard]] static auto ParseRequestBody(
      const web::http::http_request &request,
      const std::optional<json::TypeVariant> &endpoint_request_body) -> Body {
    if (!endpoint_request_body.has_value()) {
      return Body{};
    }

    const auto json = request.extract_json().get();
    auto request_body = endpoint_request_body->ParseAnyFromJson(json);

    if (const auto missing_mandatory_object =
            !request_body.HasValue() && !endpoint_request_body->IsOptional()) {
      throw std::runtime_error{"Request misses mandatory body " +
                               json.serialize()};
    }

    return request_body;
  }

  [[nodiscard]] static auto ConvertResponseBodyToJson(
      const Result &response_body,
      const std::optional<json::TypeVariant> &endpoint_response_body)
      -> web::json::value {
    if (!endpoint_response_body.has_value()) {
      return web::json::value{};
    }

    const auto no_mandatory_result =
        !response_body.HasValue() && !endpoint_response_body->IsOptional();
    ABSL_ASSERT(!no_mandatory_result && "Response misses mandatory body");

    auto json = endpoint_response_body->ConvertAnyToJson(response_body);

    const auto not_converted = !json.has_value();
    ABSL_ASSERT(!not_converted && "Response body has wrong type");

    return std::move(*json);
  }

  [[nodiscard]] auto RequestHandler(
      const web::http::http_request &request) const -> RequestHandlerResult {
    Logger().debug("Got {} request on {}", request.method(),
                   request.request_uri().path());

    const auto *endpoint = (const Endpoint *){};
    auto parsed_params = Params{};
    auto parsed_request_body = Body{};

    try {
      endpoint = &FindEndpoint(request);

      const auto raw_params = web::uri::split_query(
          web::uri::decode(request.request_uri().query()));
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

    auto response_body = Result{};

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

Server::Server(const LocalUri &uri, std::vector<Endpoint> endpoints)
    : impl_{std::make_unique<Impl>(uri, std::move(endpoints))} {}

Server::Server(Server &&) noexcept = default;

auto Server::operator=(Server &&) noexcept -> Server & = default;

Server::~Server() = default;
}  // namespace stonks::network