/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "core_sps_stream_handle.h"

#include <future>
#include <mutex>
#include <not_null.hpp>
#include <optional>
#include <type_traits>
#include <utility>

#include "core_sps_price_recorder.h"
#include "cpp_not_null.h"
#include "cpp_timer_builder.h"

namespace vh::stk::core::sps {
struct StreamHandle::Impl {
  cpp::Opt<networkx::WebSocket<&sps::PriceRecorder::RecordAsPrice>>
      web_socket{};
  std::mutex mutex{};
  // cppcheck-suppress unusedStructMember
  bool disconnected{};
};

StreamHandle::StreamHandle(Symbol symbol, absl::Duration reattempt_interval,
                           sps::StreamFactory stream_factory)
    : impl_{cpp::MakeNnSp<Impl>()},
      connect_to_web_socket_timer_{
          cpp::Execute([symbol = std::move(symbol),
                        stream_factory = std::move(stream_factory),
                        impl = impl_]() mutable {
            const auto lock = std::lock_guard{impl->mutex};

            if (!impl->disconnected) {
              impl->web_socket = stream_factory.Create(symbol);
            }
          })
              .Once()
              .IfThrowsReattemptEvery(reattempt_interval)
              .Start()} {}

auto StreamHandle::Disconnect() const -> std::future<void> {
  return std::async([impl = impl_]() {
    const auto lock = std::lock_guard{impl->mutex};
    impl->web_socket.reset();
    impl->disconnected = true;
  });
}
}  // namespace vh::stk::core::sps