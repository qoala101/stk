#include "network_aprh_ws_handler_variant.h"

#include <coroutine>
#include <gsl/assert>
#include <type_traits>
#include <utility>
#include <variant>

namespace stonks::network::aprh {
auto WsHandlerVariant::operator()(WsMessage message) -> cppcoro::task<> {
  co_await std::visit(
      [&message](auto &v) -> cppcoro::task<> {
        Expects(!v.empty());

        using V = std::remove_cvref_t<decltype(v)>;

        if constexpr (std::is_same_v<V, Handler>) {
          co_await v();
        } else if constexpr (std::is_same_v<V, HandlerWithWsMessage>) {
          co_await v(AutoParsableWsMessage{std::move(message)});
        } else {
          Expects(false);
        }
      },
      value);
}
}  // namespace stonks::network::aprh