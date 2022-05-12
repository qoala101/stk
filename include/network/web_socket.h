#ifndef STONKS_WEB_SOCKET_H_
#define STONKS_WEB_SOCKET_H_

#include <cpprest/json.h>
#include <cpprest/uri_builder.h>
#include <cpprest/ws_client.h>

#include <functional>
#include <future>
#include <string_view>

namespace stonks::rest {
class WebSocket {
 public:
  explicit WebSocket(std::string_view uri);

  explicit WebSocket(const WebSocket &) = delete;
  auto operator=(const WebSocket &) -> WebSocket & = delete;

  WebSocket(WebSocket &&) = default;
  auto operator=(WebSocket &&) -> WebSocket & = default;

  ~WebSocket();

  void AppendUri(std::string_view uri);

  void Connect();

  void Disconnect();

  void SendMessage(std::string_view message);

  enum class ReceiveMessagesResult { kContinue, kStop };
  using MessageHandler =
      std::function<ReceiveMessagesResult(const web::json::value &)>;

  auto ReceiveMessages(MessageHandler message_handler)
      -> std::shared_future<void>;

 private:
  web::websockets::client::websocket_client websocket_client_{};
  web::uri_builder uri_builder_{};

  std::shared_future<void> shared_future_{};
};
}  // namespace stonks::rest

#endif  // STONKS_WEB_SOCKET_H_
