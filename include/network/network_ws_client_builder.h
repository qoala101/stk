#ifndef STONKS_NETWORK_NETWORK_WS_CLIENT_BUILDER_H_
#define STONKS_NETWORK_NETWORK_WS_CLIENT_BUILDER_H_

#include <cppcoro/task.hpp>
#include <utility>

#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "cpp_smart_pointers.h"
#include "network_auto_parsable.h"
#include "network_i_ws_client.h"
#include "network_typed_ws_endpoint.h"

namespace stonks::network {
/**
 * @brief Convenience API for building web socket client.
 * @remark Keeps web socket connection while alive.
 */
class WsClientBuilder {
 public:
  /**
   * @brief Connects to web socket and starts receiving messages.
   * @param endpoint Web socket URI and expected types.
   */
  WsClientBuilder(TypedWsEndpoint endpoint, cpp::NnUp<IWsClient> ws_client);

  /**
   * @brief Receives message from the socket as auto parsable of valid type.
   */
  auto Receive [[nodiscard]] () -> cppcoro::task<AutoParsable>;

  /**
   * @brief Sends the object to the previously connected web socket.
   */
  template <Convertible T>
  auto Send [[nodiscard]] (T &&t) const -> cppcoro::task<> {
    co_await Send(ConvertToJson(std::forward<T>(t)));
  }

 private:
  auto Send(WsMessage message) const -> cppcoro::task<>;

  cpp::NnUp<IWsClient> ws_client_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_WS_CLIENT_BUILDER_H_
