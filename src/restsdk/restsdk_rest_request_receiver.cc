#include "restsdk_rest_request_receiver.h"

#include <cpprest/base_uri.h>
#include <cpprest/http_headers.h>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <fmt/core.h>
#include <polymorphic_value.h>
#include <pplx/pplx.h>
#include <pplx/pplxtasks.h>

#include <coroutine>
#include <cppcoro/single_consumer_event.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>
#include <gsl/assert>
#include <map>
#include <memory>
#include <nameof.hpp>
#include <not_null.hpp>
#include <optional>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/transform.hpp>
#include <string>
#include <string_view>
#include <utility>

#include "cpp_not_null.h"
#include "cpp_polymorphic_value.h"
#include "cpp_typed_struct.h"
#include "cpprest/http_msg.h"
#include "network_i_json.h"
#include "network_types.h"
#include "restsdk_json.h"
#include "restsdk_json_native_handle.h"
#include "restsdk_parse_json_fom_string.h"

namespace stonks::restsdk {
namespace {
auto MethodFrom [[nodiscard]] (const web::http::method &method) {
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

auto HttpStatusFrom [[nodiscard]] (network::Status status) {
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

auto EndpointFrom [[nodiscard]] (const web::http::http_request &request) {
  return network::Endpoint{.method = MethodFrom(request.method()),
                           .uri = {request.relative_uri().path()}};
}

auto ParamsFrom [[nodiscard]] (const std::string &request_query) {
  const auto raw_params =
      web::uri::split_query(web::uri::decode(request_query));

  return ranges::views::transform(raw_params,
                                  [](const auto &raw_param) {
                                    const auto &[name, value] = raw_param;
                                    return std::make_pair(
                                        name, ParseJsonFromString(value));
                                  }) |
         ranges::to<network::Params>;
}

auto HeadersFrom
    [[nodiscard]] (const web::http::http_headers &request_headers) {
  return std::map<std::string, std::string>{request_headers.begin(),
                                            request_headers.end()};
}

auto BodyFrom [[nodiscard]] (const web::http::http_request &request)
-> network::Body {
  auto json = request.extract_json().get();

  if (json.is_null()) {
    return std::nullopt;
  }

  return cpp::MakePv<network::IJson, Json>(
      network::IJson::NativeHandle{std::move(json)});
}

auto RestRequestFrom [[nodiscard]] (const web::http::http_request &request) {
  return network::RestRequest{
      .endpoint = EndpointFrom(request),
      .params = ParamsFrom(request.request_uri().query()),
      .headers = HeadersFrom(request.headers()),
      .body = BodyFrom(request)};
}

auto HttpResponseFrom [[nodiscard]] (const network::RestResponse &response) {
  auto http_response =
      web::http::http_response{HttpStatusFrom(response.status)};

  if (response.result.has_value()) {
    http_response.set_body(*(*response.result)->GetNativeHandle());
  }

  return http_response;
}

auto HandleHttpRequest
    [[nodiscard]] (const network::IRestRequestHandler &handler,
                   log::ILogger &logger, const web::http::http_request &request)
    -> cppcoro::task<> {
  const auto request_uri = request.absolute_uri().to_string();
  logger.LogEvent(
      fmt::format("Received {} request on {}", request.method(), request_uri));

  auto rest_request = RestRequestFrom(request);

  const auto rest_response =
      co_await handler.HandleRequestAndGiveResponse(std::move(rest_request));
  const auto http_response = HttpResponseFrom(rest_response);

  auto replied_to_request = cppcoro::single_consumer_event{};

  request.reply(http_response).then([&replied_to_request]() {
    replied_to_request.set();
  });

  co_await replied_to_request;
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
  logger_->LogImportantEvent(
      fmt::format("Stopping REST receiver on {}...", uri));

  http_listener_->close().wait();

  logger_->LogImportantEvent(fmt::format("Stopped REST receiver on {}", uri));
}

void RestRequestReceiver::Receive(
    network::Uri uri, cpp::NnUp<network::IRestRequestHandler> handler) {
  Expects(http_listener_ == nullptr);

  http_listener_ =
      cpp::MakeUp<web::http::experimental::listener::http_listener>(
          std::move(*uri));
  http_listener_->support(
      [handler = cpp::NnSp<network::IRestRequestHandler>{std::move(handler)},
       logger = logger_](const auto &request) {
        cppcoro::sync_wait(HandleHttpRequest(*handler, *logger, request));
      });

  logger_->LogImportantEvent(
      fmt::format("Starting REST receiver on {}...", *uri));

  http_listener_->open().wait();

  logger_->LogImportantEvent(fmt::format("Started REST receiver on {}", *uri));

  Ensures(http_listener_ != nullptr);
}
}  // namespace stonks::restsdk