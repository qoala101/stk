#ifndef STONKS_NETWORK_NETWORK_WS_CONNECTION_H_
#define STONKS_NETWORK_NETWORK_WS_CONNECTION_H_

#include <cppcoro/task.hpp>
#include <utility>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "network_concepts.h"  // IWYU pragma: keep
#include "network_i_ws_client.h"
#include "network_json_base_conversions.h"
#include "network_ws_types.h"

namespace stonks::network {
class WsClientBuilder;

/**
 * @brief Web socket facade which allows sending of objects
 * and keeps connection alive.
 */
class WsConnection {
 public:
  /**
   * @brief Sends the object to the previously connected web socket.
   */
  template <Convertible T>
  auto Send [[nodiscard]] (T &&t) const -> cppcoro::task<> {
    co_await Send(ConvertToJson(std::forward<T>(t)));
  }

 private:
  friend class cpp::ExposePrivateConstructorsTo<WsClientBuilder, WsConnection>;

  explicit WsConnection(cpp::NnUp<IWsClient> ws_client);

  auto Send [[nodiscard]] (WsMessage message) const -> cppcoro::task<>;

  cpp::NnUp<IWsClient> ws_client_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_WS_CONNECTION_H_