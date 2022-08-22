#ifndef STONKS_RESTSDK_RESTSDK_REST_REQUEST_RECEIVER_H_
#define STONKS_RESTSDK_RESTSDK_REST_REQUEST_RECEIVER_H_

#include <string_view>

#include "cpp_not_null.h"
#include "network_i_rest_request_handler.h"
#include "network_i_rest_request_receiver.h"

namespace web::http {
class http_request;

namespace experimental::listener {
class http_listener;
}  // namespace experimental::listener
}  // namespace web::http

namespace stonks::restsdk {
/**
 * @copyright network::IRestRequestReceiver
 */
class RestRequestReceiver : public network::IRestRequestReceiver {
 public:
  explicit RestRequestReceiver(std::string_view local_uri,
                               cpp::NnUp<network::IRestRequestHandler> handler);

  RestRequestReceiver(const RestRequestReceiver &other) = delete;
  RestRequestReceiver(RestRequestReceiver &&) noexcept;

  auto operator=(const RestRequestReceiver &other)
      -> RestRequestReceiver & = delete;
  auto operator=(RestRequestReceiver &&) noexcept -> RestRequestReceiver &;

  ~RestRequestReceiver() override;

 private:
  void HandleHttpRequest(const web::http::http_request &request) const;

  cpp::NnUp<network::IRestRequestHandler> handler_;
  cpp::NnUp<web::http::experimental::listener::http_listener> http_listener_;
};
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_REST_REQUEST_RECEIVER_H_
