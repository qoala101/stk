#include "restsdk_rest_request_receiver.h"

#include <bits/exception.h>
#include <cpprest/base_uri.h>
#include <cpprest/http_headers.h>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <fmt/format.h>
#include <polymorphic_value.h>
#include <pplx/pplxtasks.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <gsl/assert>
#include <map>
#include <memory>
#include <optional>
#include <string>

#include "cpprest/http_msg.h"
#include "network_i_json.h"
#include "network_types.h"
#include "not_null.hpp"
#include "restsdk_json.h"
#include "restsdk_json_impl.h"

namespace stonks::restsdk {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger = spdlog::stdout_color_mt("RestRequestReceiver");
  return *logger;
}

[[nodiscard]] auto NetworkMethodFromHttpMethod(const web::http::method &method)
    -> network::Method {
  if (method == web::http::methods::GET) {
    return network::Method::kGet;
  }
  if (method == web::http::methods::POST) {
    return network::Method::kPost;
  }
  if (method == web::http::methods::DEL) {
    return network::Method::kDelete;
  }

  return network::Method::kOther;
}

[[nodiscard]] auto HttpStatusFromNetworkStatus(network::Status status)
    -> web::http::status_code {
  switch (status) {
    case network::Status::kOk:
      return web::http::status_codes::OK;
    case network::Status::kBadRequest:
      return web::http::status_codes::BadRequest;
    case network::Status::kNotFound:
      return web::http::status_codes::NotFound;
    case network::Status::kInternalError:
      return web::http::status_codes::InternalError;
    default:
      Expects(false);
  }
}

[[nodiscard]] auto FetchEndpoint(const web::http::http_request &request)
    -> network::Endpoint {
  return {.method = NetworkMethodFromHttpMethod(request.method()),
          .uri = request.request_uri().path()};
}

[[nodiscard]] auto FetchParams(const std::string &request_query)
    -> network::Params {
  const auto raw_params =
      web::uri::split_query(web::uri::decode(request_query));
  auto params = network::Params{};

  for (const auto &[name, value] : raw_params) {
    params[name] = isocpp_p0201::make_polymorphic_value<network::IJson, Json>(
        network::IJson::Impl{web::json::value::parse(value)});
  }

  return params;
}

[[nodiscard]] auto FetchHeaders(const web::http::http_headers &request_headers)
    -> std::map<std::string, std::string> {
  auto headers = std::map<std::string, std::string>{};

  for (const auto &header : request_headers) {
    headers[header.first] = header.second;
  }

  return headers;
}

[[nodiscard]] auto FetchBody(const web::http::http_request &request)
    -> network::Body {
  auto json = request.extract_json().get();

  if (json.is_null()) {
    return std::nullopt;
  }

  return isocpp_p0201::make_polymorphic_value<network::IJson, Json>(
      network::IJson::Impl{std::move(json)});
}

[[nodiscard]] auto FetchRequestData(const web::http::http_request &request)
    -> network::RestRequestData {
  return {.params = FetchParams(request.request_uri().query()),
          .headers = FetchHeaders(request.headers()),
          .body = FetchBody(request)};
}
}  // namespace

RestRequestReceiver::RestRequestReceiver(
    std::string_view local_uri,
    std::function<std::pair<network::Status, network::Result>(
        network::Endpoint, network::RestRequestData)>
        handler)
    : handler_{[&handler]() {
        Expects(handler);
        return std::move(handler);
      }()},
      http_listener_{[this, local_uri]() {
        auto http_listener = cpp::assume_not_null(
            std::make_unique<web::http::experimental::listener::http_listener>(
                local_uri.data()));
        http_listener->support(
            std::bind_front(&RestRequestReceiver::HandleHttpRequest, this));
        http_listener->open();
        return http_listener;
      }()} {
  Ensures(handler_);
}

RestRequestReceiver::RestRequestReceiver(RestRequestReceiver &&) noexcept =
    default;

auto RestRequestReceiver::operator=(RestRequestReceiver &&) noexcept
    -> RestRequestReceiver & = default;

RestRequestReceiver::~RestRequestReceiver() = default;

void RestRequestReceiver::HandleHttpRequest(
    const web::http::http_request &request) const {
  Logger().info("Received {} request on {}", request.method(),
                request.absolute_uri().path());

  auto endpoint = FetchEndpoint(request);
  auto data = FetchRequestData(request);
  auto [status, result] = handler_(std::move(endpoint), std::move(data));

  const auto response = [status = status, &result = result]() {
    auto response =
        web::http::http_response{HttpStatusFromNetworkStatus(status)};

    if (result.has_value()) {
      response.set_body((*result)->GetImpl().GetJson());
    }

    return response;
  }();
  request.reply(response);
}
}  // namespace stonks::restsdk