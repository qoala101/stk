#ifndef STONKS_REST_REQUEST_H_
#define STONKS_REST_REQUEST_H_

#include <cpprest/http_msg.h>
#include <cpprest/json.h>
#include <cpprest/uri_builder.h>

#include <boost/lexical_cast.hpp>
#include <magic_enum.hpp>
#include <optional>
#include <string_view>

#include "concepts.h"

namespace stonks::rest {
  class RestRequest {
   public:
    explicit RestRequest(std::string_view uri);
    explicit RestRequest(web::http::method method, std::string_view uri);

    RestRequest &AppendUri(std::string_view uri);

    RestRequest &AddParameter(std::string_view key, std::string_view value);

    template <Number T>
    RestRequest &AddParameter(std::string_view key, T value) {
      return AddParameter(key, boost::lexical_cast<std::string>(value));
    }

    template <Enumeration T>
    RestRequest &AddParameter(std::string_view key, T value) {
      return AddParameter(key, magic_enum::enum_name<T>(value));
    }

    template <typename T>
    RestRequest &AddParameter(std::string_view key,
                              const std::optional<T> &optional_value) {
      if (optional_value.has_value()) {
        return AddParameter(key, *optional_value);
      }

      return *this;
    }

    RestRequest &AddHeader(std::string_view key, std::string_view value);

    std::string GetUri() const;
    const std::string &GetParametersAsString() const;

    std::optional<web::json::value> SendAndGetResponse() const;

   private:
    web::http::http_request http_request_{};
    web::uri_builder uri_builder_{};
  };
}  // namespace stonks::rest

#endif  // STONKS_REST_REQUEST_H_
