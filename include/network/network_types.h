#ifndef STONKS_NETWORK_NETWORK_TYPES_H_
#define STONKS_NETWORK_NETWORK_TYPES_H_

#include <map>
#include <memory>
#include <string>

#include "network_enums.h"
#include "network_json.h"

namespace stonks::network {
/**
 * @brief HTTP endpoint.
 */
struct Endpoint {
  Method method;
  std::string uri;
};

/**
 * @brief Data to be sent with request.
 */
struct RestRequestData {
  std::map<std::string, std::string> params{};
  std::map<std::string, std::string> headers{};
  std::unique_ptr<IJson> body{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_TYPES_H_
