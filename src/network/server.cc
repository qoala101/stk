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
#include "json.h"
#include "json_conversions.h"
#include "type_variant.h"

namespace stonks::network {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger = spdlog::stdout_color_mt("Server");
  return *logger;
}

struct RequestHandlerResult {
  web::http::status_code status_code{};
  web::json::value response_body{};
};

/**
 * @throws If parameters are wrong.
 */
[[nodiscard]] auto ParseParams(
    const std::map<std::string, std::string> &raw_params,
    const std::map<std::string, Optional> &endpoint_params)
    -> params {
  auto parsed_params = params{};

  for (const auto &endpoint_param : endpoint_params) {
    const auto &[param_name, param_type] = endpoint_param;
    const auto raw_param = raw_params.find(param_name);

    if (const auto no_param = raw_param == raw_params.end()) {
      if (param_type == Optional::kMandatory) {
        throw std::runtime_error{"Request misses mandatory parameter " +
                                 param_name};
      }

      parsed_params[param_name] = {};

      continue;
    }

    const auto &raw_value = raw_param->second;
    parsed_params[param_name] = web::json::value::parse(raw_value);
  }

  return parsed_params;
}

/**
 * @throws If request body is wrong.
 */
[[nodiscard]] auto ParseRequestBody(
    const web::http::http_request &request,
    Optional endpoint_request_body) -> Body {
  const auto request_json = request.extract_json().get();

  if (const auto no_mandatory_body =
          request_json.is_null() &&
          (endpoint_request_body == Optional::kMandatory)) {
    throw std::runtime_error{"Request doesn't have mandatory body"};
  }

  return request_json;
}

[[nodiscard]] auto ConvertResponseBodyToJson(
    const Result &response_body, Optional endpoint_response_body)
    -> const web::json::value & {
  const auto &response_json = response_body.GetJson();

  const auto no_mandatory_result =
      response_json.is_null() &&
      (endpoint_response_body == Optional::kMandatory);
  ABSL_ASSERT(!no_mandatory_result && "Response doesn't have mandatory body");

  return response_json;
}
}  // namespace

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

      Logger().info("Replying {}", result.status_code);
      request.reply(response);
    });
    http_listener_.open();
  }

  Impl(const Impl &) = delete;
  auto operator=(const Impl &) -> Impl & = delete;

  Impl(Impl &&) noexcept = default;
  auto operator=(Impl &&) noexcept -> Impl & = default;

  ~Impl() { http_listener_.close().wait(); }

 private:
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

  [[nodiscard]] auto RequestHandler(
      const web::http::http_request &request) const -> RequestHandlerResult {
    Logger().info("Got {} request on {}", request.method(),
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

    auto response_json = ConvertResponseBodyToJson(
        response_body, endpoint->desc.response_body);
    return RequestHandlerResult{.status_code = web::http::status_codes::OK,
                                .response_body = std::move(response_json)};
  }

  web::http::experimental::listener::http_listener http_listener_{};
  std::vector<Endpoint> endpoints_{};
};

Server::Server(const LocalUri &uri, std::vector<Endpoint> endpoints)
    : impl_{std::make_unique<Impl>(uri, std::move(endpoints))} {}

Server::Server(Server &&) noexcept = default;

auto Server::operator=(Server &&) noexcept -> Server & = default;

Server::~Server() = default;
}  // namespace stonks::network