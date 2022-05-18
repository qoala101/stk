#ifndef STONKS_NETWORK_CLIENT_H_
#define STONKS_NETWORK_CLIENT_H_

#include <cpprest/json.h>

#include <memory>
#include <optional>
#include <string_view>

#include "client_server_types.h"

namespace stonks {
class Client {
 public:
  virtual ~Client();

 protected:
  explicit Client(std::string_view base_uri);

  /**
   * @throws std::exception if inputs or result dont match endpoint description
   */
  auto Execute(const EndpointDesc &endpoint,
               const std::map<std::string, json::Any> &params,
               const stonks::json::Any &object) const -> std::any;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_{};
};
}  // namespace stonks

#endif  // STONKS_NETWORK_CLIENT_H_
