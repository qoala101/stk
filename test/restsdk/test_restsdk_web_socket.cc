#include <absl/time/clock.h>
#include <absl/time/time.h>
#include <gtest/gtest.h>

#include <concepts>
#include <exception>
#include <range/v3/algorithm/find_if.hpp>
#include <variant>

#include "cpp_concepts.h"
#include "cpp_not_null.h"
#include "cpp_polymorphic_value.h"
#include "log_i_logger.h"
#include "network_i_json.h"
#include "network_i_web_socket_client.h"
#include "network_i_web_socket_handler.h"
#include "network_json_basic_conversions.h"
#include "network_json_common_conversions.h"
#include "network_json_conversions_facades.h"
#include "network_typed_endpoint.h"
#include "network_types.h"
#include "network_web_socket_client_builder.h"
#include "restsdk_web_socket_client.h"
#include "test_restsdk_injector.h"

namespace {
struct BinanceWebSocketMessage {
  std::string method{};
  const std::vector<std::string> params{{"ethusdt@bookTicker"}};
  const int id{1};

 private:
  [[nodiscard]] friend auto operator==(const BinanceWebSocketMessage &,
                                       const BinanceWebSocketMessage &)
      -> bool = default;
};

using MessageVariant = std::variant<std::string, BinanceWebSocketMessage>;
}  // namespace

namespace stonks::network {
template <>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> BinanceWebSocketMessage {
  return MakeFromJson<BinanceWebSocketMessage>(json, "method", "params", "id");
}
}  // namespace stonks::network

namespace {
auto ConvertToJson(const BinanceWebSocketMessage &value)
    -> stonks::cpp::Pv<stonks::network::IJson> {
  return stonks::network::BuildJsonFrom("method", value.method, "params",
                                        value.params, "id", value.id);
}

class WebSocketHandler : public stonks::network::IWebSocketHandler {
 public:
  void HandleMessage(stonks::network::WsMessage message) const override {
    try {
      messages_.emplace_back(
          stonks::network::ParseFromJson<MessageVariant>(*message));
    } catch (const std::exception &) {
    }
  }

  [[nodiscard]] auto GetMessages() const
      -> const std::vector<MessageVariant> & {
    return messages_;
  }

 private:
  mutable std::vector<MessageVariant> messages_{};
};

const auto kEndpoint = stonks::network::WsEndpoint{
    "wss://demo.piesocket.com/v3/"
    "channel_1?api_key=VCXCEuvhGcBDP7XhiJJUDvR1e1D3eiVjgZ9VRiaV&notify_"
    "self"};
const auto kMessage1 = BinanceWebSocketMessage{.method = "SUBSCRIBE"};
const auto kMessage2 = BinanceWebSocketMessage{.method = "UNSUBSCRIBE"};

TEST(WebSocket, SendAndReceive) {
  const auto handler = stonks::cpp::MakeNnSp<WebSocketHandler>();

  {
    auto web_socket =
        test::restsdk::Injector()
            .create<stonks::cpp::NnUp<stonks::network::IWebSocketClient>>();

    web_socket->Connect(kEndpoint);
    web_socket->SetMessageHandler(handler);

    web_socket->SendMessage(ConvertToJson(kMessage1));
    web_socket->SendMessage(ConvertToJson(kMessage2));
    absl::SleepFor(absl::Seconds(2));
  }

  const auto &received_messages = handler->GetMessages();

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
}

static const auto kTypedSocket = stonks::network::TypedWsEndpoint{
    .endpoint = kEndpoint,
    .expected_types = {
        .received_message =
            stonks::network::ExpectedType<BinanceWebSocketMessage>(),
        .sent_message =
            stonks::network::ExpectedType<BinanceWebSocketMessage>()}};

TEST(WebSocket, Facade) {
  auto received_messages = std::vector<BinanceWebSocketMessage>{};

  const auto client = test::restsdk::Injector()
                          .create<stonks::network::WebSocketClientBuilder>()
                          .Handling([&received_messages](
                                        stonks::network::AutoParsable message) {
                            received_messages.emplace_back(message);
                          })
                          .On(kTypedSocket)
                          .Connect();
  client.Send(kMessage1);
  client.Send(kMessage2);
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
}
}  // namespace