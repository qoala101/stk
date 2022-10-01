#ifndef STONKS_NETWORK_NETWORK_WEB_SOCKET_CLIENT_BUILDER_H_
#define STONKS_NETWORK_NETWORK_WEB_SOCKET_CLIENT_BUILDER_H_

#include <utility>

#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "cpp_smart_pointers.h"
#include "network_auto_parsable_web_socket_handler.h"
#include "network_i_web_socket_client.h"
#include "network_i_web_socket_handler.h"
#include "network_typed_endpoint.h"
#include "network_web_socket_sender.h"

namespace stonks::network {
/**
 * @brief Convenience API for building web socket client.
 */
class WebSocketClientBuilder {
 public:
  explicit WebSocketClientBuilder(
      cpp::NnUp<IWebSocketClient> web_socket_client);

  /**
   * @brief Sets web socket URI and expected types.
   */
  auto On(TypedWsEndpoint endpoint) -> WebSocketClientBuilder &;

  /**
   * @brief Registers handler for received messages.
   */
  template <typename T>
    requires std::constructible_from<AutoParsableWebSocketHandler, T>
  auto Handling(T &&handler) -> WebSocketClientBuilder & {
    return Handling(
        cpp::MakeNnUp<AutoParsableWebSocketHandler>(std::forward<T>(handler)));
  }

  /**
   * @brief Connects to web socket and starts receiving messages.
   * @return Keeps web socket connection while alive.
   * @remark Other methods should not be called after this.
   */
  [[nodiscard]] auto Connect() -> WebSocketSender;

 private:
  [[nodiscard]] auto Handling(cpp::NnUp<IWebSocketHandler> handler)
      -> WebSocketClientBuilder &;

  cpp::Up<IWebSocketClient> web_socket_client_{};
  cpp::Opt<TypedWsEndpoint> endpoint_{};
  cpp::Up<IWebSocketHandler> handler_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_WEB_SOCKET_CLIENT_BUILDER_H_
