#ifndef VH_NETWORK_APRH_WS_HANDLER_VARIANT_H_
#define VH_NETWORK_APRH_WS_HANDLER_VARIANT_H_

#include <cppcoro/task.hpp>
#include <function2/function2.hpp>

#include "cpp_variant_struct.h"
#include "network_aprh_handler_variant.h"
#include "network_auto_parsable_ws_message.h"
#include "network_ws_types.h"

namespace vh::network::aprh {
using HandlerWithWsMessage =
    fu2::unique_function<auto(AutoParsableWsMessage)->cppcoro::task<>>;

/**
 * @brief Variant of auto-parsable web socket message handler.
 */
struct WsHandlerVariant
    : public cpp::VariantStruct<Handler, HandlerWithWsMessage> {
  /**
   * @brief Calls operator of the current handler variant.
   */
  auto operator() [[nodiscard]] (WsMessage message) -> cppcoro::task<>;
};
}  // namespace vh::network::aprh

#endif  // VH_NETWORK_APRH_WS_HANDLER_VARIANT_H_
