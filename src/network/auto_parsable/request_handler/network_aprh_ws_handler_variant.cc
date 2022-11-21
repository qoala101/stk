#include "network_aprh_ws_handler_variant.h"

#include <gsl/assert>
#include <type_traits>
#include <utility>
#include <variant>

namespace stonks::network::aprh {
void WsHandlerVariant::operator()(WsMessage message) {
  std::visit(
      [&message](auto &v) {
        Expects(!v.empty());

        using V = std::remove_cvref_t<decltype(v)>;

        if constexpr (std::is_same_v<V, Handler>) {
          v();
        } else if constexpr (std::is_same_v<V, HandlerWithWsMessage>) {
          v(AutoParsable{std::move(message)});
        } else {
          Expects(false);
        }
      },
      value);
}
}  // namespace stonks::network::aprh