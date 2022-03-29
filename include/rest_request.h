#ifndef STONKS_REST_REQUEST_H_
#define STONKS_REST_REQUEST_H_

#include <cpprest/http_msg.h>
#include <cpprest/json.h>
#include <cpprest/uri_builder.h>

#include <optional>
#include <string_view>

namespace stonks::rest {
class RestRequest {
 public:
  explicit RestRequest(std::string_view uri);
  explicit RestRequest(web::http::method method, std::string_view uri);

  RestRequest &AddParameter(std::string_view key, std::string_view value);
  RestRequest &AddHeader(std::string_view key, std::string_view value);

  const std::string &GetParametersAsString() const;

  std::optional<web::json::value> SendAndGetResponse() const;

 private:
  web::http::http_request http_request_{};
  web::uri_builder uri_builder_{};
};
}  // namespace stonks::rest

#endif  // STONKS_REST_REQUEST_H_
