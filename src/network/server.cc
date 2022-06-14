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

[[nodiscard]] auto ParseParams(const web::http::http_request &request)
    -> Params {
  const auto &query = request.request_uri().query();
  const auto raw_params = web::uri::split_query(web::uri::decode(query));
  auto parsed_params = Params{};

  for (const auto &[name, value] : raw_params) {
    parsed_params[name] = Json{web::json::value::parse(value)};
  }

  return parsed_params;
}

[[nodiscard]] auto ParseBody(const web::http::http_request &request) -> Body {
  return Body{request.extract_json().get()};
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

      request.reply(response);
    });
    http_listener_.open();
  }

  Impl(const Impl &) = delete;
  Impl(Impl &&) noexcept = default;

  auto operator=(const Impl &) -> Impl & = delete;
  auto operator=(Impl &&) noexcept -> Impl & = default;

  ~Impl() { http_listener_.close().wait(); }

 private:
  void PrependBaseUriToEndpoints() {
    const auto &base_uri = http_listener_.uri().path();

    for (auto &endpoint : endpoints_) {
      endpoint.desc.relative_uri.insert(0, base_uri);
    }
  }

  [[nodiscard]] auto FindEndpoint(const web::http::http_request &request) const
      -> const Endpoint * {
    const auto endpoint = ranges::find_if(
        endpoints_, [&request_method = request.method(),
                     &request_relative_uri = request.request_uri().path()](
                        const Endpoint &endpoint) {
          return (endpoint.desc.method == request_method) &&
                 (endpoint.desc.relative_uri == request_relative_uri);
        });

    if (const auto no_such_endpoint = endpoint == endpoints_.end()) {
      return nullptr;
    }

    return endpoint.base();
  }

  [[nodiscard]] auto RequestHandler(
      const web::http::http_request &request) const -> RequestHandlerResult {
    Logger().info("Got {} request on {}", request.method(),
                  request.request_uri().path());

    const auto *endpoint = FindEndpoint(request);

    if (const auto endpoint_not_found = endpoint == nullptr) {
      Logger().info("Endpoint not found {}", request.request_uri().path());
      return {.status_code = web::http::status_codes::BadRequest};
    }

    auto request_params = ParseParams(request);
    auto request_body = ParseBody(request);
    auto response_body = Result{};

    try {
      response_body =
          endpoint->handler(std::move(request_params), std::move(request_body));
    } catch (const std::exception &exception) {
      Logger().error("Request handler thrown exception {}", exception.what());

      return RequestHandlerResult{
          .status_code = web::http::status_codes::InternalError,
          .response_body =
              json::ConvertToJson(std::runtime_error{exception.what()})};
    }

    return RequestHandlerResult{
        .status_code = web::http::status_codes::OK,
        .response_body = std::move(response_body).GetJson()};
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