#ifndef STONKS_NETWORK_NETWORK_REST_SERVER_H_
#define STONKS_NETWORK_NETWORK_REST_SERVER_H_

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include "network_auto_parsable_request_handler.h"
#include "network_i_factory.h"
#include "network_i_rest_request_receiver.h"
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
  auto Handling(Endpoint endpoint, T handler) -> RestServer & {
    return std::move(*this).Handling(
        std::move(endpoint), AutoParsableRequestHandler{std::move(handler)});
  }

  /**
   * @copydoc Handling
   */
  auto Handling(Endpoint endpoint, AutoParsableRequestHandler handler)
      -> RestServer &;

  /**
   * @brief Starts the server and returns request receiver
   * responsible for handling of requests.
   * @return Keeps handling REST requests while alive.
   */
  auto Start() -> cpp::not_null<std::unique_ptr<IRestRequestReceiver>>;

 private:
  cpp::not_null<std::unique_ptr<IFactory>> network_factory_;
  std::optional<std::string> base_uri_{};
  std::map<Endpoint, RestRequestHandler> endpoint_handlers_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_SERVER_H_
