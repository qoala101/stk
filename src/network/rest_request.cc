#include "rest_request.h"

#include <cpprest/http_client.h>
#include <spdlog/spdlog.h>

#include <magic_enum.hpp>

namespace stonks::rest {
RestRequest::RestRequest(std::string_view uri)
    : RestRequest{web::http::methods::GET, uri} {}

RestRequest::RestRequest(web::http::method method, std::string_view uri)
    : http_request_{method}, uri_builder_{std::string{uri}} {}

RestRequest &RestRequest::AppendUri(std::string_view uri) {
  uri_builder_.append(std::string{uri});
  return *this;
}

RestRequest &RestRequest::AddParameter(std::string_view key,
                                       std::string_view value) {
  uri_builder_.append_query(std::string{key}, std::string{value});
  return *this;
}

RestRequest &RestRequest::AddParameter(std::string_view key,
                                       std::chrono::milliseconds value) {
  return AddParameter(key, value.count());
}

RestRequest &RestRequest::AddHeader(std::string_view key,
                                    std::string_view value) {
  http_request_.headers().add(std::string{key}, std::string{value});
  return *this;
}

auto RestRequest::any_AddParameterValue(std::string_view key,
                                        const json::Any &value)
    -> RestRequest & {
  const auto string_value = std::visit(
      [&value = value.value](const auto &variant) {
        return variant.any_ConvertToJson(value).serialize();
      },
      value.converter);

  AddParameter(key, string_value);
  return *this;
}

RestRequest &RestRequest::SetJson(const web::json::value &json) {
  http_request_.set_body(json);
  return *this;
}

std::string RestRequest::GetUri() const { return uri_builder_.to_string(); }

const std::string &RestRequest::GetParametersAsString() const {
  return uri_builder_.query();
}

std::optional<web::json::value> RestRequest::SendAndGetResponse() const {
  const auto uri = uri_builder_.to_uri();
  spdlog::info("Sending {} request to {}", http_request_.method(),
               uri.to_string());

  auto json = std::optional<web::json::value>{};

  try {
    auto http_client = web::http::client::http_client{uri};
    const auto response = http_client.request(http_request_).get();
    json = response.extract_json().get();
  } catch (...) {
    spdlog::error("Request failed");
    return std::nullopt;
  }

  spdlog::info("Got response of type {}", magic_enum::enum_name(json->type()));
  return json;
}
}  // namespace stonks::rest