#ifndef STONKS_NETWORK_CLIENT_SERVER_TYPES_H_
#define STONKS_NETWORK_CLIENT_SERVER_TYPES_H_

#include <cpprest/http_msg.h>
#include <cpprest/json.h>

#include <any>
#include <chrono>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <typeinfo>
#include <variant>
#include <vector>

#include "json_conversions.h"

namespace stonks {
struct EndpointDesc {
  web::http::method method{};
  std::string relative_uri{};
  std::map<std::string, json::AnyDesc> params{};
  std::optional<json::AnyDesc> object{};
  std::optional<json::AnyDesc> result{};

  // TODO(vh): move to other struct. It is not needed by the client
  // stonks::json::Any
  std::function<std::any(std::map<std::string, json::Any>, stonks::json::Any)>
      handler_{};
};
}  // namespace stonks

#endif  // STONKS_NETWORK_CLIENT_SERVER_TYPES_H_
