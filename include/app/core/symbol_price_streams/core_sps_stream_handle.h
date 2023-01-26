#ifndef STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SPS_STREAM_HANDLE_H_
#define STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SPS_STREAM_HANDLE_H_

#include "core_sps_stream_factory.h"
#include "cpp_timer.h"

namespace stonks::core::sps {
/**
 * @brief Keeps web socket connection which redirects received Binance
 * book ticks to price recorder.
 */
class StreamHandle {
 public:
  /**
   * @brief Creates connection and starts receiving book ticks.
   * Would reattempt connection if it fails.
   */
  StreamHandle(Symbol symbol, absl::Duration reattempt_interval,
               sps::StreamFactory stream_factory);

 private:
  cpp::NnSp<cpp::Opt<networkx::WebSocket<&sps::PriceRecorder::RecordAsPrice>>>
      web_socket_;
  cpp::Timer connect_to_web_socket_timer_;
};
}  // namespace stonks::core::sps

#endif  // STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SPS_STREAM_HANDLE_H_
