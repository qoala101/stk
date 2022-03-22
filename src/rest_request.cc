#include "rest_request.h"

#include <cpprest/http_client.h>

#include <spdlog/spdlog.h>

namespace stonks::rest
{
  RestRequest::RestRequest(const std::string &uri, web::http::method method)
      : method_{method}, uri_builder_{uri}
  {
  }

  RestRequest &RestRequest::WithParameter(const std::string &key, const std::string &value)
  {
    uri_builder_.append_query(key, value);
    return *this;
  }

  std::optional<web::json::value> RestRequest::SendAndGetResponse() const
  {
    const auto uri = uri_builder_.to_uri();
    spdlog::info("Sending {} request to {}", method_, uri.to_string());

    try
    {
      auto http_client = web::http::client::http_client{uri};
      const auto response = http_client.request(method_).get();
      return response.extract_json().get();
    }
    catch (...)
    {
      spdlog::error("Request failed");
    }

    return std::nullopt;
  }
}