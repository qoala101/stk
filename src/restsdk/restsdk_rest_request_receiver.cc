#include "restsdk_rest_request_receiver.h"

#include <cpprest/base_uri.h>
#include <cpprest/http_headers.h>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <fmt/core.h>
#include <polymorphic_value.h>
#include <pplx/pplxtasks.h>

#include <gsl/assert>
#include <map>
#include <memory>
#include <nameof.hpp>
#include <not_null.hpp>
#include <optional>
#include <string>
#include <utility>

#include "cpp_not_null.h"
#include "cpp_polymorphic_value.h"
#include "cpprest/http_msg.h"
#include "network_enums.h"
#include "network_i_json.h"
#include "network_types.h"
#include "restsdk_json.h"
#include "restsdk_json_native_handle.h"
#include "restsdk_parse_json_fom_string.h"

namespace stonks::restsdk {
namespace {
[[nodiscard]] auto MethodFrom(const web::http::method &method) {
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

[[nodiscard]] auto HttpStatusFrom(network::Status status) {
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

[[nodiscard]] auto EndpointFrom(const web::http::http_request &request) {
  return network::Endpoint{.method = MethodFrom(request.method()),
                           .uri = {request.relative_uri().path()}};
}

[[nodiscard]] auto ParamsFrom(const std::string &request_query) {
  const auto raw_params =
      web::uri::split_query(web::uri::decode(request_query));
  auto params = network::Params{};

  for (const auto &[name, value] : raw_params) {
    params.emplace(name, ParseJsonFromString(value));
  }

  return params;
}

[[nodiscard]] auto HeadersFrom(const web::http::http_headers &request_headers) {
  auto headers = std::map<std::string, std::string>{};

  for (const auto &header : request_headers) {
    headers.emplace(header.first, header.second);
  }

  return headers;
}

[[nodiscard]] auto BodyFrom(const web::http::http_request &request)
    -> network::Body {
  auto json = request.extract_json().get();

  if (json.is_null()) {
    return std::nullopt;
  }

  return cpp::MakePv<network::IJson, Json>(
      network::IJson::NativeHandle{std::move(json)});
}

[[nodiscard]] auto RestRequestFrom(const web::http::http_request &request) {
  return network::RestRequest{
      .endpoint = EndpointFrom(request),
      .params = ParamsFrom(request.request_uri().query()),
      .headers = HeadersFrom(request.headers()),
      .body = BodyFrom(request)};
}

[[nodiscard]] auto HttpResponseFrom(const network::RestResponse &response) {
  auto http_response =
      web::http::http_response{HttpStatusFrom(response.status)};

  if (response.result.has_value()) {
    http_response.set_body(*(*response.result)->GetNativeHandle());
  }

  return http_response;
}

void HandleHttpRequest(const network::IRestRequestHandler &handler,
                       log::ILogger &logger,
                       const web::http::http_request &request) {
  const auto request_uri = request.absolute_uri().path();
  logger.LogEvent(
      fmt::format("Received {} request on {}", request.method(), request_uri));

  auto rest_request = RestRequestFrom(request);
  const auto rest_response =
      handler.HandleRequestAndGiveResponse(std::move(rest_request));
  const auto http_response = HttpResponseFrom(rest_response);
  request.reply(http_response).wait();

  logger.LogEvent(fmt::format("Replied {} on {}",
                              nameof::nameof_enum(rest_response.status),
                              request_uri));
}
}  // namespace

RestRequestReceiver::RestRequestReceiver(cpp::NnUp<log::ILogger> logger)
    : logger_{std::move(logger)} {}

RestRequestReceiver::RestRequestReceiver(RestRequestReceiver &&) noexcept =
    default;

auto RestRequestReceiver::operator=(RestRequestReceiver &&) noexcept
    -> RestRequestReceiver & = default;

RestRequestReceiver::~RestRequestReceiver() {
  if (const auto object_was_moved = http_listener_ == nullptr) {
    return;
  }

  const auto uri = http_listener_->uri().to_string();
  logger_->LogImportantEvent(fmt::format("Stopping REST receiver: {}...", uri));

  http_listener_->close().wait();

  logger_->LogImportantEvent(fmt::format("Stopped REST receiver: {}", uri));
}

void RestRequestReceiver::Receive(
    network::Uri uri, cpp::NnUp<network::IRestRequestHandler> handler) {
  Expects(http_listener_ == nullptr);

  http_listener_ =
      cpp::MakeUp<web::http::experimental::listener::http_listener>(
          std::move(uri.value));
  http_listener_->support(
      [handler = cpp::NnSp<network::IRestRequestHandler>{std::move(handler)},
       logger = logger_](const web::http::http_request &request) {
        HandleHttpRequest(*handler, *logger, request);
      });

  logger_->LogImportantEvent(
      fmt::format("Starting REST receiver: {}...", uri.value));

  http_listener_->open().wait();

  logger_->LogImportantEvent(
      fmt::format("Started REST receiver: {}", uri.value));

  Ensures(http_listener_ != nullptr);
}
}  // namespace stonks::restsdk