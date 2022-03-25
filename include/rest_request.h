#ifndef STONKS_REST_REQUEST_H_
#define STONKS_REST_REQUEST_H_

#include <string>
#include <optional>

#include <cpprest/json.h>
#include <cpprest/http_msg.h>
#include <cpprest/uri_builder.h>

namespace stonks::rest
{
  class RestRequest
  {
  public:
    explicit RestRequest(const std::string &uri, web::http::method method = web::http::methods::GET);

    RestRequest &WithParameter(const std::string &key, const std::string &value);
    RestRequest &WithHeader(const std::string &key, const std::string &value);

    std::string ParametersAsString() const;

    std::optional<web::json::value> SendAndGetResponse() const;

  private:
    web::http::http_request http_request_{};
    web::uri_builder uri_builder_{};
  };
}

#endif // STONKS_REST_REQUEST_H_
