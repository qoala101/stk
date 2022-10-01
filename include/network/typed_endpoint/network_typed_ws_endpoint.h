#ifndef STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_WS_ENDPOINT_H_
#define STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_WS_ENDPOINT_H_

#include <function2/function2.hpp>
#include <map>
#include <string>

#include "network_typed_endpoint.h"
#include "network_ws_types.h"

namespace stonks::network {
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
}  // namespace stonks::network

#endif  // STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_WS_ENDPOINT_H_
