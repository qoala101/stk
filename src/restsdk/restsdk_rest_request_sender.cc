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
#include <optional>
#include <string>
#include <utility>

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

auto ConvertToRequestParam(const network::IJson &json) -> std::string {
  const auto &rest_json = json.GetImpl().GetJson();

  if (rest_json.is_string()) {
    return rest_json.as_string();
  }

  return rest_json.serialize();
}
}  // namespace

RestRequestSender::RestRequestSender(network::Endpoint endpoint)
    : endpoint_{std::move(endpoint)} {}

auto RestRequestSender::SendRequestAndGetResponse(
    const network::RestRequestData &data) const
    -> std::pair<network::Status,
                 isocpp_p0201::polymorphic_value<network::IJson>> {
  auto uri_builder = [this, &data]() {
    auto uri_builder = web::http::uri_builder{endpoint_.uri};

    for (const auto &[key, value] : data.params) {
      uri_builder.append_query(key, ConvertToRequestParam(*value));
    }

    return uri_builder;
  }();

  auto http_request = [this, &data]() {
    auto http_request =
        web::http::http_request{HttpMethodFromNetworkMethod(endpoint_.method)};

    for (const auto &[key, value] : data.headers) {
      http_request.headers().add(key, value);
    }

    if (data.body) {
      http_request.set_body((*data.body)->GetImpl().GetJson());
    }

    return http_request;
  }();

  const auto full_uri = uri_builder.to_uri();

  Logger().info("Sending {} request to {}",
                magic_enum::enum_name(endpoint_.method), full_uri.to_string());

  auto http_client = web::http::client::http_client{full_uri};
  const auto response = http_client.request(http_request).get();
  auto json = response.extract_json().get();

  return {NetworkStatusFromHttpStatus(response.status_code()),
          isocpp_p0201::make_polymorphic_value<network::IJson, Json>(
              network::IJson::Impl{std::move(json)})};
}
}  // namespace stonks::restsdk