/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_WS_CONNECTION_H_
#define VH_NETWORK_WS_CONNECTION_H_

#include <cppcoro/task.hpp>
#include <utility>

#include "cpp_not_null.h"
#include "network_concepts.h"  // IWYU pragma: keep
#include "network_i_ws_client.h"
#include "network_json_base_conversions.h"
#include "network_ws_types.h"

namespace vh::network {
/**
 * @brief Web socket facade which allows sending of objects
 * and keeps connection alive.
 */
class WsConnection {
 public:
  explicit WsConnection(cpp::NnUp<IWsClient> ws_client);

  /**
   * @brief Sends the object to the previously connected web socket.
   */
  template <Convertible T>
  auto Send [[nodiscard]] (T &&t) const -> cppcoro::task<> {
    co_await Send(ConvertToJson(std::forward<T>(t)));
  }

 private:
  auto Send [[nodiscard]] (WsMessage message) const -> cppcoro::task<>;

  cpp::NnUp<IWsClient> ws_client_;
};
}  // namespace vh::network

#endif  // VH_NETWORK_WS_CONNECTION_H_
