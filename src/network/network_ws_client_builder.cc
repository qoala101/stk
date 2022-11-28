#include "network_ws_client_builder.h"

#include <cppcoro/task.hpp>
#include <gsl/assert>
#include <memory>
#include <not_null.hpp>
#include <type_traits>

#include "cpp_not_null.h"
#include "network_auto_parsable.h"
#include "network_typed_ws_client.h"
#include "network_typed_ws_endpoint.h"

namespace stonks::network {
WsClientBuilder::WsClientBuilder(TypedWsEndpoint endpoint,
                                 cpp::NnUp<IWsClient> ws_client)
    : ws_client_{cpp::MakeNnUp<TypedWsClient>(
          std::move(endpoint.expected_types), std::move(ws_client))} {
  ws_client_->Connect(std::move(endpoint.endpoint));
}

auto WsClientBuilder::Receive() -> cppcoro::task<AutoParsable> {
  co_return AutoParsable{co_await ws_client_->ReceiveMessage()};
}

auto WsClientBuilder::Send(WsMessage message) const -> cppcoro::task<> {
  co_await ws_client_->SendMessage(std::move(message));
}
}  // namespace stonks::network