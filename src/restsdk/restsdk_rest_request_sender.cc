#include "restsdk_rest_request_sender.h"

#include <cpprest/base_uri.h>
#include <cpprest/http_client.h>
#include <cpprest/http_headers.h>
#include <cpprest/http_msg.h>
#include <cpprest/json.h>
#include <cpprest/uri_builder.h>
#include <fmt/core.h>
#include <polymorphic_value.h>
#include <pplx/pplx.h>
#include <pplx/pplxtasks.h>

#include <coroutine>
#include <cppcoro/single_consumer_event.hpp>
#include <exception>
#include <gsl/assert>
#include <map>
#include <memory>
#include <nameof.hpp>
#include <not_null.hpp>
#include <string>
#include <string_view>
#include <utility>

#include "cpp_message_exception.h"
#include "cpp_polymorphic_value.h"
#include "cpp_typed_struct.h"
#include "network_i_json.h"
#include "network_types.h"
#include "restsdk_call_as_coroutine.h"
#include "restsdk_json.h"
#include "restsdk_json_native_handle.h"

namespace stonks::restsdk {
namespace {
auto HttpMethodFrom [[nodiscard]] (network::Method method) {
  switch (method) {
    case network::Method::kGet:
      return web::http::methods::GET;
    case network::Method::kPost:
      return web::http::methods::POST;
    case network::Method::kDelete:
      return web::http::methods::DEL;
    case network::Method::kOther:
      Expects(false);
  }

  Expects(false);
}

auto StatusFrom [[nodiscard]] (web::http::status_code status) {
  switch (status) {
    case web::http::status_codes::OK:
      return network::Status::kOk;
    case web::http::status_codes::BadRequest:
      return network::Status::kBadRequest;
    case web::http::status_codes::NotFound:
      return network::Status::kNotFound;
    case web::http::status_codes::InternalError:
      return network::Status::kInternalError;
    default:
      break;
  }

  if ((status >= web::http::status_codes::Continue) &&
      (status <= web::http::status_codes::SwitchingProtocols)) {
    return network::Status::kOtherInfo;
  }

  if ((status >= web::http::status_codes::OK) &&
      (status <= web::http::status_codes::IMUsed)) {
    return network::Status::kOtherSuccess;
  }

  if ((status >= web::http::status_codes::MultipleChoices) &&
      (status <= web::http::status_codes::PermanentRedirect)) {
    return network::Status::kOtherRedirection;
  }

  if ((status >= web::http::status_codes::BadRequest) &&
      (status <= web::http::status_codes::UnavailableForLegalReasons)) {
    return network::Status::kOtherClientError;
  }

  if ((status >= web::http::status_codes::InternalError) &&
      (status <= web::http::status_codes::NetworkAuthenticationRequired)) {
    return network::Status::kOtherServerError;
  }

  Expects(false);
}

auto ConvertToRequestParam [[nodiscard]] (const network::IJson &json) {
  const auto &rest_json = json.GetNativeHandle();

  if (rest_json->is_string()) {
    return rest_json->as_string();
  }

  return rest_json->serialize();
}

auto HttpRequestFrom [[nodiscard]] (const network::RestRequest &request) {
  auto http_request =
      web::http::http_request{HttpMethodFrom(request.endpoint.method)};

  for (const auto &[key, value] : request.headers) {
    http_request.headers().add(key, value);
  }

  if (request.body) {
    http_request.set_body(*request.body->GetNativeHandle());
  }

  return http_request;
}

auto WebUriFrom [[nodiscard]] (const network::RestRequest &request) {
  auto uri_builder = web::http::uri_builder{*request.endpoint.uri};

  for (const auto &[key, value] : request.params) {
    uri_builder.append_query(key, ConvertToRequestParam(*value));
  }

  return uri_builder.to_uri();
}
}  // namespace

RestRequestSender::RestRequestSender(cpp::NnUp<log::ILogger> logger)
    : logger_{std::move(logger)} {}

auto RestRequestSender::SendRequestAndGetResponse(network::RestRequest request)
    const -> cppcoro::task<network::RestResponse> {
  const auto full_uri = WebUriFrom(request);

  logger_->LogImportantEvent(fmt::format(
      "Sending {} request to {}", nameof::nameof_enum(request.endpoint.method),
      full_uri.to_string()));

  auto http_client = web::http::client::http_client{full_uri};
  const auto http_request = HttpRequestFrom(request);

  auto response = network::RestResponse{};

  try {
    co_await CallAsCoroutine(
        http_client.request(http_request)
            .then([&response](const web::http::http_response &http_response) {
              response.status = StatusFrom(http_response.status_code());
              return http_response.extract_json();
            })
            .then([&response](web::json::value http_response_json) {
              response.result = cpp::MakePv<network::IJson, Json>(
                  network::IJson::NativeHandle{std::move(http_response_json)});
            }));
  } catch (const std::exception &e) {
    logger_->LogErrorCondition(
        fmt::format("{} request to {} failed: {}",
                    nameof::nameof_enum(request.endpoint.method),
                    full_uri.to_string(), e.what()));
    throw;
  }

  co_return response;
}
}  // namespace stonks::restsdk