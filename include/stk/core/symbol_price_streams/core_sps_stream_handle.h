#ifndef VH_STK_CORE_SPS_STREAM_HANDLE_H_
#define VH_STK_CORE_SPS_STREAM_HANDLE_H_

#include <absl/time/time.h>

#include "core_sps_price_recorder.h"
#include "core_sps_stream_factory.h"
#include "core_types.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "cpp_timer.h"
#include "networkx_web_socket.h"

namespace vh::stk::core::sps {
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
}  // namespace vh::stk::core::sps

#endif  // VH_STK_CORE_SPS_STREAM_HANDLE_H_
