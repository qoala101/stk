#include "restsdk_rest_request_sender.h"

#include <cpprest/asyncrt_utils.h>
#include <cpprest/base_uri.h>
#include <cpprest/http_client.h>
#include <cpprest/http_headers.h>
#include <cpprest/http_msg.h>
#include <cpprest/json.h>
#include <cpprest/uri_builder.h>
#include <fmt/core.h>
#include <polymorphic_value.h>
#include <pplx/pplxtasks.h>

#include <gsl/assert>
#include <map>
#include <memory>
#include <nameof.hpp>
#include <utility>

#include "cpp_polymorphic_value.h"
#include "cpp_typed_struct.h"
#include "network_enums.h"
#include "network_i_json.h"
#include "network_types.h"
#include "not_null.hpp"
#include "restsdk_json.h"
#include "restsdk_json_native_handle.h"

namespace stonks::restsdk {
namespace {
[[nodiscard]] auto HttpMethodFrom(network::Method method) {
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

[[nodiscard]] auto StatusFrom(web::http::status_code status) {
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

[[nodiscard]] auto ConvertToRequestParam(const network::IJson &json) {
  const auto &rest_json = json.GetNativeHandle();

  if (rest_json->is_string()) {
    return rest_json->as_string();
  }

  return rest_json->serialize();
}

[[nodiscard]] auto HttpRequestFrom(const network::RestRequest &request) {
  auto http_request =
      web::http::http_request{HttpMethodFrom(request.endpoint.method)};

  for (const auto &[key, value] : request.headers) {
    http_request.headers().add(key, value);
  }

  if (request.body) {
    http_request.set_body(*(*request.body)->GetNativeHandle());
  }

  return http_request;
}

[[nodiscard]] auto WebUriFrom(const network::RestRequest &request) {
  auto uri_builder = web::http::uri_builder{*request.endpoint.uri};

  for (const auto &[key, value] : request.params) {
    uri_builder.append_query(key, ConvertToRequestParam(*value));
  }

  return uri_builder.to_uri();
}
}  // namespace

RestRequestSender::RestRequestSender(cpp::NnUp<log::ILogger> logger)
    : logger_{std::move(logger)} {}

auto RestRequestSender::SendRequestAndGetResponse(
    network::RestRequest request) const -> network::RestResponse {
  const auto full_uri = WebUriFrom(request);

  logger_->LogImportantEvent(fmt::format(
      "Sending {} request to {}", nameof::nameof_enum(request.endpoint.method),
      full_uri.to_string()));

  auto http_client = web::http::client::http_client{full_uri};
  const auto http_request = HttpRequestFrom(request);
  const auto http_response = http_client.request(http_request).get();

  auto response =
      network::RestResponse{.status = StatusFrom(http_response.status_code())};
  auto http_response_json = http_response.extract_json().get();

  if (!http_response_json.is_null()) {
    response.result = cpp::MakePv<network::IJson, Json>(
        network::IJson::NativeHandle{std::move(http_response_json)});
  }

  return response;
}
}  // namespace stonks::restsdk