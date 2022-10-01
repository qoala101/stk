#ifndef STONKS_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_NETWORK_APRH_WS_HANDLER_VARIANT_H_
#define STONKS_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_NETWORK_APRH_WS_HANDLER_VARIANT_H_

#include <function2/function2.hpp>

#include "cpp_variant_struct.h"
#include "network_aprh_handler_variant.h"
#include "network_auto_parsable.h"
#include "network_ws_types.h"

namespace stonks::network::aprh {
using HandlerWithWsMessage = fu2::unique_function<void(AutoParsable)>;

/**
 * @brief Variant of auto-parsable web socket message handler.
 */
struct WsHandlerVariant
    : public cpp::VariantStruct<Handler, HandlerWithWsMessage> {
  /**
   * @brief Calls operator of the current handler variant.
   */
  void operator()(WsMessage message);
};
}  // namespace stonks::network::aprh

#endif  // STONKS_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_NETWORK_APRH_WS_HANDLER_VARIANT_H_
