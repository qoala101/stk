#ifndef STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SYMBOL_PRICES_STREAM_H_
#define STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SYMBOL_PRICES_STREAM_H_

#include <absl/time/time.h>

#include "core_sps_book_tick_handler.h"
#include "core_sps_book_tick_web_socket_factory.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "cpp_timer.h"
#include "networkx_web_socket.h"

namespace stonks::core {
/**
 * @brief While alive, receives single symbol prices from Binance book ticks
 * web socket and records them to Symbols DB.
 */
class SymbolPricesStream {
 public:
  /**
   * @brief Creates connection and starts receiving book ticks.
   * @param reattempt_interval  Time in which to reattempt connection
   * if it fails.
   */
  SymbolPricesStream(sps::BookTickWebSocketFactory web_socket_factory,
                     absl::Duration reattempt_interval);

 private:
  cpp::NnSp<cpp::Opt<networkx::WebSocket<&sps::BookTickHandler::RecordAsPrice>>>
      web_socket_;
  cpp::Timer connect_to_web_socket_timer_;
};
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SYMBOL_PRICES_STREAM_H_
