#include "core_symbol_prices_stream.h"

#include <cppcoro/sync_wait.hpp>
#include <optional>
#include <type_traits>
#include <utility>

#include "core_sps_book_tick_handler.h"
#include "cpp_not_null.h"
#include "cpp_timer_builder.h"

namespace stonks::core {
SymbolPricesStream::SymbolPricesStream(
    sps::BookTickWebSocketFactory web_socket_factory,
    absl::Duration reattempt_interval)
    : web_socket_{cpp::MakeNnSp<cpp::Opt<
          networkx::WebSocket<&sps::BookTickHandler::RecordAsPrice>>>()},
      connect_to_web_socket_timer_{
          cpp::Execute([web_socket = web_socket_,
                        web_socket_factory = std::move(web_socket_factory)]() {
            *web_socket = cppcoro::sync_wait(web_socket_factory.Create());
          })
              .Once()
              .IfThrowsReattemptEvery(reattempt_interval)
              .Start()} {}

}  // namespace stonks::core