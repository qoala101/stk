#ifndef STONKS_NETWORK_NETWORK_WS_TYPES_H_
#define STONKS_NETWORK_NETWORK_WS_TYPES_H_

#include <compare>
#include <map>
#include <string>

#include "cpp_optional.h"
#include "cpp_polymorphic_value.h"
#include "cpp_typed_struct.h"
#include "network_i_json.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Web socket message.
 */
using WsMessage = cpp::Pv<IJson>;

/**
 * @brief Web socket endpoint.
 */
struct WsEndpoint : public cpp::TypedStruct<Uri> {};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_WS_TYPES_H_
