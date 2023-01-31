#ifndef VH_NETWORK_I_WS_CLIENT_H_
#define VH_NETWORK_I_WS_CLIENT_H_

#include <cppcoro/task.hpp>

#include "cpp_not_null.h"
#include "network_i_ws_message_handler.h"
#include "network_ws_types.h"

namespace vh::network {
/**
 * @brief Allows communication with an arbitrary web socket.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IWsClient {
 public:
  virtual ~IWsClient() = default;

  /**
   * @brief Connects to the web socket on the specified URI.
   */
  virtual void Connect(WsEndpoint endpoint) = 0;

  /**
   * @brief Redirects received messages to the handler.
   */
  virtual void SetMessageHandler(cpp::NnUp<IWsMessageHandler> handler) = 0;

  /**
   * @brief Sends message to the connected socket.
   */
  virtual auto SendMessage [[nodiscard]] (WsMessage message) const
      -> cppcoro::task<> = 0;
};
}  // namespace vh::network

#endif  // VH_NETWORK_I_WS_CLIENT_H_
