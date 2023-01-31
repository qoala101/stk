#include "core_sps_stream_handle.h"

#include <not_null.hpp>
#include <optional>
#include <type_traits>
#include <utility>

#include "core_sps_price_recorder.h"
#include "cpp_timer_builder.h"

namespace vh::stk::core::sps {
StreamHandle::StreamHandle(Symbol symbol, absl::Duration reattempt_interval,
                           sps::StreamFactory stream_factory)
    : web_socket_{cpp::MakeNnSp<
          cpp::Opt<networkx::WebSocket<&sps::PriceRecorder::RecordAsPrice>>>()},
      connect_to_web_socket_timer_{
          cpp::Execute([symbol = std::move(symbol),
                        stream_factory = std::move(stream_factory),
                        web_socket = web_socket_]() mutable {
            *web_socket = stream_factory.Create(symbol);
          })
              .Once()
              .IfThrowsReattemptEvery(reattempt_interval)
              .Start()} {}
}  // namespace vh::stk::core::sps