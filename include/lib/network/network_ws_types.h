/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_WS_TYPES_H_
#define VH_NETWORK_WS_TYPES_H_

#include <compare>
#include <map>
#include <string>

#include "cpp_polymorphic_value.h"
#include "cpp_typed_struct.h"
#include "network_i_json.h"
#include "network_types.h"

namespace vh::network {
/**
 * @brief Web socket message.
 */
using WsMessage = cpp::Pv<IJson>;

/**
 * @brief Web socket endpoint.
 */
struct WsEndpoint : public cpp::TypedStruct<Uri> {};
}  // namespace vh::network

#endif  // VH_NETWORK_WS_TYPES_H_
