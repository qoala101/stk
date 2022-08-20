#ifndef STONKS_NETWORK_NETWORK_REST_SERVER_H_
#define STONKS_NETWORK_NETWORK_REST_SERVER_H_

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "network_auto_parsable_request_handler.h"
#include "network_i_factory.h"
#include "network_i_rest_request_handler.h"
#include "network_i_rest_request_receiver.h"
#include "network_typed_endpoint.h"
#include "network_types.h"
#include "not_null.hpp"

namespace stonks::network {
/**
 * @brief Convenience API for building server-like REST request listener.
 */
class RestServer {
 public:
  explicit RestServer(cpp::not_null<std::unique_ptr<IFactory>> network_factory);

  /**
   * @brief Sets base URI on which requests are to be handled.
   */
  auto On(std::string base_uri) -> RestServer &;

  /**
   * @brief Registers handler for particular endpoint.
   */
  template <typename T>
  requires std::is_constructible_v<AutoParsableRequestHandler, T>
  auto Handling(TypedEndpoint endpoint, T handler) -> RestServer & {
    return Handling(
        std::move(endpoint),
        cpp::assume_not_null(
            std::make_unique<AutoParsableRequestHandler>(std::move(handler))));
  }

  /**
   * @brief Starts the server and returns request receiver
   * responsible for handling of requests.
   * @return Keeps handling REST requests while alive.
   * @remark Other methods should not be called after this.
   */
  [[nodiscard]] auto Start()
      && -> cpp::not_null<std::unique_ptr<IRestRequestReceiver>>;

 private:
  [[nodiscard]] auto Handling(
      TypedEndpoint endpoint,
      cpp::not_null<std::unique_ptr<IRestRequestHandler>> handler)
      -> RestServer &;

  cpp::not_null<std::unique_ptr<IFactory>> network_factory_;
  std::optional<std::string> base_uri_{};
  std::map<Endpoint, cpp::not_null<std::unique_ptr<IRestRequestHandler>>>
      endpoint_handlers_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_SERVER_H_
