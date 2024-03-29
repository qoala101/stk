/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_TYPED_WS_ENDPOINT_H_
#define VH_NETWORK_TYPED_WS_ENDPOINT_H_

#include <function2/function2.hpp>
#include <map>
#include <string>

#include "network_typed_endpoint.h"
#include "network_ws_types.h"

namespace vh::network {
/**
 * @brief Expected web socket message types.
 */
struct WsEndpointTypes {
  ParseTypeCheck received_message{};
  ParseTypeCheck sent_message{};
};

/**
 * @brief Web socket endpoint which received and sent messages to be validated
 * to have expected types prior to processing.
 */
struct TypedWsEndpoint {
  WsEndpoint endpoint{};
  WsEndpointTypes expected_types{};
};
}  // namespace vh::network

#endif  // VH_NETWORK_TYPED_WS_ENDPOINT_H_
