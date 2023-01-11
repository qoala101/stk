#include "core_sps_stream_handle.h"

#include "cpp_timer_builder.h"

namespace stonks::core::sps {
StreamHandle::StreamHandle(
    Symbol symbol, absl::Duration reattempt_interval,
    cpp::NnSp<sps::BookTickWebSocketFactory> web_socket_factory)
    : connect_to_web_socket_timer_{
          cpp::Execute(
              [symbol = std::move(symbol),
               web_socket_factory = std::move(web_socket_factory),
               web_socket = cpp::Opt<networkx::WebSocket<
                   &sps::BookTickHandler::RecordAsPrice>>{}]() mutable {
                web_socket =
                    cppcoro::sync_wait(web_socket_factory->Create(symbol));
              })
              .Once()
              .IfThrowsReattemptEvery(reattempt_interval)
              .Start()} {}
}  // namespace stonks::core::sps