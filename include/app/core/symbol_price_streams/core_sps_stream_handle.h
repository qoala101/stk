#ifndef STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SPS_STREAM_HANDLE_H_
#define STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SPS_STREAM_HANDLE_H_

#include "core_sps_book_tick_web_socket_factory.h"
#include "cpp_timer.h"

namespace stonks::core::sps {
/**
 * @brief While alive, receives single symbol prices from Binance book ticks
 * web socket and records them to Symbols DB.
 */
class StreamHandle {
 public:
  /**
   * @brief Creates connection and starts receiving book ticks.
   * Would reattempt connection if it fails.
   */
  StreamHandle(sps::BookTickWebSocketFactory web_socket_factory,
               absl::Duration reattempt_interval);

 private:
  cpp::NnSp<cpp::Opt<networkx::WebSocket<&sps::BookTickHandler::RecordAsPrice>>>
      web_socket_;
  cpp::Timer connect_to_web_socket_timer_;
};
}  // namespace stonks::core::sps

#endif  // STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SPS_STREAM_HANDLE_H_
