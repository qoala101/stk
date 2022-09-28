#include <absl/time/clock.h>
#include <absl/time/time.h>
#include <gtest/gtest.h>

#include <exception>
#include <range/v3/algorithm/find_if.hpp>
#include <variant>

#include "cpp_concepts.h"
#include "cpp_not_null.h"
#include "cpp_polymorphic_value.h"
#include "network_i_json.h"
#include "network_i_web_socket_client.h"
#include "network_i_web_socket_handler.h"
#include "network_json_basic_conversions.h"
#include "network_json_common_conversions.h"
#include "network_json_conversions_facades.h"
#include "network_types.h"
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

auto ConvertToJson(const BinanceWebSocketMessage &value) -> cpp::Pv<IJson> {
  return BuildJsonFrom("method", value.method, "params", value.params, "id",
                       value.id);
}
}  // namespace stonks::network

namespace {
class WebSocketHandler : public stonks::network::IWebSocketHandler {
 public:
  void HandleMessage(stonks::network::Message message) const override {
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

TEST(WebSocket, Test1) {
  const auto handler = stonks::cpp::MakeNnSp<WebSocketHandler>();
  const auto message1 = BinanceWebSocketMessage{.method = "SUBSCRIBE"};
  const auto message2 = BinanceWebSocketMessage{.method = "UNSUBSCRIBE"};

  {
    auto web_socket =
        test::restsdk::Injector()
            .create<stonks::cpp::NnUp<stonks::network::IWebSocketClient>>();

    web_socket->Connect(
        {"wss://demo.piesocket.com/v3/"
         "channel_1?api_key=VCXCEuvhGcBDP7XhiJJUDvR1e1D3eiVjgZ9VRiaV&notify_"
         "self"},
        handler);

    absl::SleepFor(absl::Seconds(2));
    web_socket->SendMessage(stonks::network::ConvertToJson(message1));
    web_socket->SendMessage(stonks::network::ConvertToJson(message2));
    absl::SleepFor(absl::Seconds(2));
  }

  const auto &received_messages = handler->GetMessages();

  const auto message1_received =
      ranges::find_if(received_messages, [&message1](const auto &message) {
        return std::holds_alternative<BinanceWebSocketMessage>(message) &&
               (std::get<BinanceWebSocketMessage>(message) == message1);
      }) != received_messages.end();
  EXPECT_TRUE(message1_received);

  const auto message2_received =
      ranges::find_if(received_messages, [&message2](const auto &message) {
        return std::holds_alternative<BinanceWebSocketMessage>(message) &&
               (std::get<BinanceWebSocketMessage>(message) == message2);
      }) != received_messages.end();
  EXPECT_TRUE(message2_received);
}
}  // namespace