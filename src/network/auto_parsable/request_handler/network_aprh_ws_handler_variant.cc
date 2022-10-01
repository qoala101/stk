#include "network_aprh_ws_handler_variant.h"

#include <gsl/assert>
#include <utility>
#include <variant>

namespace stonks::network::aprh {
void WsHandlerVariant::operator()(WsMessage message) {
  std::visit(
      [&message](auto &v) {
        Expects(!v.empty());

        using V = decltype(v);

        if constexpr (cpp::DecaysTo<V, Handler>) {
          v();
          return;
        }

        if constexpr (cpp::DecaysTo<V, HandlerWithWsMessage>) {
          v(AutoParsable{std::move(message)});
          return;
        }

        Expects(false);
      },
      value);
}
}  // namespace stonks::network::aprh