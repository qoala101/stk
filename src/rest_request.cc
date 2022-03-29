#include "rest_request.h"

#include <cpprest/http_client.h>
#include <spdlog/spdlog.h>

namespace stonks::rest {
RestRequest::RestRequest(std::string_view uri)
    : RestRequest{web::http::methods::GET, uri} {}

RestRequest::RestRequest(web::http::method method, std::string_view uri)
    : http_request_{method}, uri_builder_{std::string{uri}} {}

RestRequest &RestRequest::AddParameter(std::string_view key,
                                       std::string_view value) {
  uri_builder_.append_query(std::string{key}, std::string{value});
  return *this;
}

RestRequest &RestRequest::AddHeader(std::string_view key,
                                    std::string_view value) {
  http_request_.headers().add(std::string{key}, std::string{value});
  return *this;
}

const std::string &RestRequest::GetParametersAsString() const {
  return uri_builder_.query();
}

std::optional<web::json::value> RestRequest::SendAndGetResponse() const {
  const auto uri = uri_builder_.to_uri();
  spdlog::info("Sending {} request to {}", http_request_.method(),
               uri.to_string());

  try {
    auto http_client = web::http::client::http_client{uri};
    const auto response = http_client.request(http_request_).get();
    return response.extract_json().get();
  } catch (...) {
    spdlog::error("Request failed");
  }

  return std::nullopt;
}
}  // namespace stonks::rest