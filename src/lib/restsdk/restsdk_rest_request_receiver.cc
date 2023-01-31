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
#include <magic_enum.hpp>
#include <map>
#include <memory>
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
#include "cpp_share.h"
#include "cpp_typed_struct.h"
#include "cpprest/http_msg.h"
#include "network_i_json.h"
#include "network_types.h"
#include "restsdk_call_as_coroutine.h"
#include "restsdk_json.h"
#include "restsdk_json_native_handle.h"
#include "restsdk_parse_json_fom_string.h"

namespace vh::restsdk {
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

auto NativeStatusFrom [[nodiscard]] (network::Status status) {
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
  return absl::flat_hash_map<std::string, std::string>{request_headers.begin(),
                                            request_headers.end()};
}

auto BodyFrom [[nodiscard]] (const web::http::http_request &request)
-> network::Body {
  auto native_json = request.extract_json().get();
  return cpp::MakePv<network::IJson, Json>(
      network::IJson::NativeHandle{std::move(native_json)});
}

auto RequestFrom [[nodiscard]] (const web::http::http_request &request) {
  return network::RestRequest{
      .endpoint = EndpointFrom(request),
      .params = ParamsFrom(request.request_uri().query()),
      .headers = HeadersFrom(request.headers()),
      .body = BodyFrom(request)};
}

auto NativeResponseFrom [[nodiscard]] (const network::RestResponse &response) {
  auto native_response =
      web::http::http_response{NativeStatusFrom(response.status)};

  if (!response.result->IsNull()) {
    native_response.set_body(*response.result->GetNativeHandle());
  }

  return native_response;
}

auto HandleNativeRequest
    [[nodiscard]] (network::IRestRequestHandler &handler, log::ILogger &logger,
                   const web::http::http_request &native_request)
    -> cppcoro::task<> {
  const auto request_uri = native_request.absolute_uri().to_string();
  logger.LogImportantEvent(fmt::format("Received {} request on {}",
                                       native_request.method(), request_uri));

  auto request = RequestFrom(native_request);
  const auto response =
      co_await handler.HandleRequestAndGiveResponse(std::move(request));
  const auto native_response = NativeResponseFrom(response);

  try {
    co_await CallAsCoroutine(native_request.reply(native_response));
  } catch (const std::exception &e) {
    logger.LogErrorCondition(fmt::format("Couldn't reply {} on {}: {}",
                                         magic_enum::enum_name(response.status),
                                         request_uri, e.what()));
    throw;
  }

  logger.LogImportantEvent(fmt::format(
      "Replied {} on {}", magic_enum::enum_name(response.status), request_uri));
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
  http_listener_->support([handler = cpp::Share(std::move(handler)),
                           logger = logger_](const auto &request) {
    cppcoro::sync_wait(HandleNativeRequest(*handler, *logger, request));
  });

  logger_->LogImportantEvent(
      fmt::format("Starting REST receiver on {}...", *uri));

  http_listener_->open().wait();

  logger_->LogImportantEvent(fmt::format("Started REST receiver on {}", *uri));

  Ensures(http_listener_ != nullptr);
}
}  // namespace vh::restsdk