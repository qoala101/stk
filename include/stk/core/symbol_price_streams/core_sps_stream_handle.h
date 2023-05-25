/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_STK_CORE_SPS_STREAM_HANDLE_H_
#define VH_STK_CORE_SPS_STREAM_HANDLE_H_

#include <absl/time/time.h>

#include <future>

#include "core_sps_stream_factory.h"
#include "core_types.h"
#include "cpp_not_null.h"
#include "cpp_timer.h"

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

  /**
   * @brief Disconnects web socket asynchronously.
   * @remark Web socket would be disconnected in destructor if not called
   * explicitly.
   */
  auto Disconnect [[nodiscard]] () const -> std::future<void>;

 private:
  struct Impl;

  cpp::NnSp<Impl> impl_;
  cpp::Timer connect_to_web_socket_timer_;
};
}  // namespace vh::stk::core::sps

#endif  // VH_STK_CORE_SPS_STREAM_HANDLE_H_
