#include "restsdk_rest_request_sender.h"

#include <bits/exception.h>
#include <cpprest/asyncrt_utils.h>
#include <cpprest/base_uri.h>
#include <cpprest/http_client.h>
#include <cpprest/http_headers.h>
#include <cpprest/http_msg.h>
#include <cpprest/json.h>
#include <cpprest/uri_builder.h>
#include <fmt/format.h>
#include <polymorphic_value.h>
#include <pplx/pplxtasks.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <gsl/assert>
#include <magic_enum.hpp>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "ccutils_polymorphic_value.h"
#include "network_enums.h"
#include "network_i_json.h"
#include "network_types.h"
#include "restsdk_json.h"
#include "restsdk_json_impl.h"

namespace stonks::restsdk {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger = spdlog::stdout_color_mt("Client");
  return *logger;
}

[[nodiscard]] auto HttpMethodFromNetworkMethod(network::Method method)
    -> web::http::method {
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

[[nodiscard]] auto NetworkStatusFromHttpStatus(web::http::status_code status)
    -> network::Status {
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

[[nodiscard]] auto ConvertToRequestParam(const network::IJson &json)
    -> std::string {
  const auto &rest_json = json.GetImpl().GetJson();

  if (rest_json.is_string()) {
    return rest_json.as_string();
  }

  return rest_json.serialize();
}

[[nodiscard]] auto HttpRequestFromNetworkRequest(
    const network::RestRequest &request) {
  auto http_request = web::http::http_request{
      HttpMethodFromNetworkMethod(request.endpoint.method)};

  for (const auto &[key, value] : request.headers) {
    http_request.headers().add(key, value);
  }

  if (request.body) {
    http_request.set_body((*request.body)->GetImpl().GetJson());
  }

  return http_request;
}

[[nodiscard]] auto FetchWebUriFromRequest(const network::RestRequest &request)
    -> web::uri {
  auto uri_builder = web::http::uri_builder{request.endpoint.uri};

  for (const auto &[key, value] : request.params) {
    uri_builder.append_query(key, ConvertToRequestParam(*value));
  }

  return uri_builder.to_uri();
}
}  // namespace

auto RestRequestSender::SendRequestAndGetResponse(
    network::RestRequest request) const -> network::RestResponse {
  const auto full_uri = FetchWebUriFromRequest(request);

  Logger().info("Sending {} request to {}",
                magic_enum::enum_name(request.endpoint.method),
                full_uri.to_string());

  auto http_client = web::http::client::http_client{full_uri};
  const auto http_request = HttpRequestFromNetworkRequest(request);
  const auto http_response = http_client.request(http_request).get();

  auto response = network::RestResponse{
      .status = NetworkStatusFromHttpStatus(http_response.status_code())};
  auto http_response_json = http_response.extract_json().get();

  if (!http_response_json.is_null()) {
    response.result.emplace(ccutils::MakePv<network::IJson, Json>(
        network::IJson::Impl{std::move(http_response_json)}));
  }

  return response;
}
}  // namespace stonks::restsdk