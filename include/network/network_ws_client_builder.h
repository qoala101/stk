#ifndef STONKS_NETWORK_NETWORK_WS_CLIENT_BUILDER_H_
#define STONKS_NETWORK_NETWORK_WS_CLIENT_BUILDER_H_

#include <utility>

#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "cpp_smart_pointers.h"
#include "network_auto_parsable_ws_message_handler.h"
#include "network_i_ws_client.h"
#include "network_i_ws_message_handler.h"
#include "network_typed_ws_endpoint.h"
#include "network_ws_connection.h"

namespace stonks::network {
/**
 * @brief Convenience API for building web socket client.
 */
class WsClientBuilder {
 public:
  /**
   * @param endpoint Web socket URI and expected types.
   */
  explicit WsClientBuilder(TypedWsEndpoint endpoint,
                           cpp::NnUp<IWsClient> ws_client);

  /**
   * @brief Registers handler for received messages.
   */
  template <typename T>
    requires std::constructible_from<AutoParsableWsMessageHandler, T>
  auto Handling(T &&handler) -> WsClientBuilder & {
    return Handling(
        cpp::MakeNnUp<AutoParsableWsMessageHandler>(std::forward<T>(handler)));
  }

  /**
   * @brief Connects to web socket and starts receiving messages.
   * @return Keeps web socket connection while alive.
   * @remark Other methods should not be called after this.
   */
  [[nodiscard]] auto Connect() -> WsConnection;

 private:
  [[nodiscard]] auto Handling(cpp::NnUp<IWsMessageHandler> handler)
      -> WsClientBuilder &;

  cpp::Opt<TypedWsEndpoint> endpoint_{};
  cpp::Up<IWsClient> ws_client_{};
  cpp::Up<IWsMessageHandler> handler_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_WS_CLIENT_BUILDER_H_
