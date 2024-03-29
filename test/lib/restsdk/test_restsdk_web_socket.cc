/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include <absl/time/clock.h>
#include <absl/time/time.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

#include <boost/di.hpp>
#include <coroutine>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>
#include <exception>
#include <not_null.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/range/dangling.hpp>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "cpp_not_null.h"
#include "cpp_polymorphic_value.h"
#include "gtest/gtest_pred_impl.h"
#include "network_i_json.h"
#include "network_i_ws_client.h"
#include "network_i_ws_message_handler.h"
#include "network_json_base_conversions.h"
#include "network_json_conversions_facades.h"
#include "network_typed_endpoint.h"
#include "network_typed_ws_endpoint.h"
#include "network_ws_client_builder.h"
#include "network_ws_connection.h"
#include "network_ws_types.h"
#include "test_restsdk_injector.h"

namespace {
struct BinanceWebSocketMessage {
  std::string method{};
  const std::vector<std::string> params{{"ethusdt@bookTicker"}};
  const int id{1};

 private:
  friend auto operator== [[nodiscard]] (const BinanceWebSocketMessage &,
                                        const BinanceWebSocketMessage &)
  -> bool = default;
};

using MessageVariant = std::variant<std::string, BinanceWebSocketMessage>;
}  // namespace

namespace vh::network {
template <>
auto JsonParser<BinanceWebSocketMessage>::operator()(const IJson &json) const
    -> Type {
  return MakeFromJson<Type>(json, "method", "params", "id");
}
}  // namespace vh::network

namespace {
auto ConvertToJson(const BinanceWebSocketMessage &value)
    -> vh::cpp::Pv<vh::network::IJson> {
  return vh::network::BuildJsonFrom("method", value.method, "params",
                                    value.params, "id", value.id);
}

class WebSocketHandler : public vh::network::IWsMessageHandler {
 public:
  explicit WebSocketHandler(vh::cpp::Nn<std::vector<MessageVariant> *> messages)
      : messages_{messages} {}

  auto HandleMessage [[nodiscard]] (vh::network::WsMessage message)
  -> cppcoro::task<> override {
    try {
      messages_->emplace_back(
          vh::network::ParseFromJson<MessageVariant>(*message));
    } catch (const std::exception &) {
    }

    co_return;
  }

 private:
  mutable vh::cpp::Nn<std::vector<MessageVariant> *> messages_;
};

const auto kEndpoint = vh::network::WsEndpoint{"wss://ws.postman-echo.com/raw"};
const auto kMessage1 = BinanceWebSocketMessage{.method = "SUBSCRIBE"};
const auto kMessage2 = BinanceWebSocketMessage{.method = "UNSUBSCRIBE"};

TEST(WebSocket, SendAndReceive) {
  cppcoro::sync_wait([]() -> cppcoro::task<> {
    auto received_messages = std::vector<MessageVariant>{};
    auto handler_up = vh::cpp::MakeNnUp<WebSocketHandler>(
        vh::cpp::AssumeNn(&received_messages));

    {
      auto web_socket = test::restsdk::Injector()
                            .create<vh::cpp::NnUp<vh::network::IWsClient>>();

      web_socket->Connect(kEndpoint);
      web_socket->SetMessageHandler(std::move(handler_up));

      co_await web_socket->SendMessage(ConvertToJson(kMessage1));
      co_await web_socket->SendMessage(ConvertToJson(kMessage2));
      absl::SleepFor(absl::Seconds(2));
    }

    const auto message1_received =
        ranges::find_if(received_messages, [](const auto &message) {
          return std::holds_alternative<BinanceWebSocketMessage>(message) &&
                 (std::get<BinanceWebSocketMessage>(message) == kMessage1);
        }) != received_messages.end();
    EXPECT_TRUE(message1_received);

    const auto message2_received =
        ranges::find_if(received_messages, [](const auto &message) {
          return std::holds_alternative<BinanceWebSocketMessage>(message) &&
                 (std::get<BinanceWebSocketMessage>(message) == kMessage2);
        }) != received_messages.end();
    EXPECT_TRUE(message2_received);
  }());
}

static const auto kTypedSocket = vh::network::TypedWsEndpoint{
    .endpoint = kEndpoint,
    .expected_types = {
        .received_message =
            vh::network::ExpectedType<BinanceWebSocketMessage>(),
        .sent_message = vh::network::ExpectedType<BinanceWebSocketMessage>()}};

TEST(WebSocket, Facade) {
  cppcoro::sync_wait([]() -> cppcoro::task<> {
    auto received_messages = std::vector<BinanceWebSocketMessage>{};

    const auto client =
        vh::network::WsClientBuilder{
            kTypedSocket, test::restsdk::Injector()
                              .create<vh::cpp::NnUp<vh::network::IWsClient>>()}
            .Handling([&received_messages](auto message) -> cppcoro::task<> {
              received_messages.emplace_back(*message);
              co_return;
            })
            .Connect();
    co_await client.Send(kMessage1);
    co_await client.Send(kMessage2);
    absl::SleepFor(absl::Seconds(2));

    const auto message1_received =
        ranges::find_if(received_messages, [](const auto &message) {
          return message == kMessage1;
        }) != received_messages.end();
    EXPECT_TRUE(message1_received);

    const auto message2_received =
        ranges::find_if(received_messages, [](const auto &message) {
          return message == kMessage2;
        }) != received_messages.end();
    EXPECT_TRUE(message2_received);
  }());
}
}  // namespace