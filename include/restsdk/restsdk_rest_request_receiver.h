#ifndef STONKS_RESTSDK_RESTSDK_REST_REQUEST_RECEIVER_H_
#define STONKS_RESTSDK_RESTSDK_REST_REQUEST_RECEIVER_H_

#include "cpp_not_null.h"
#include "cpp_smart_pointers.h"
#include "log_i_logger.h"
#include "network_i_rest_request_handler.h"
#include "network_i_rest_request_receiver.h"
#include "network_types.h"

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
  explicit RestRequestReceiver(cpp::NnSp<log::ILogger> logger);

  RestRequestReceiver(const RestRequestReceiver &) = delete;
  RestRequestReceiver(RestRequestReceiver &&) noexcept;

  auto operator=(const RestRequestReceiver &) -> RestRequestReceiver & = delete;
  auto operator=(RestRequestReceiver &&) noexcept -> RestRequestReceiver &;

  ~RestRequestReceiver() override;

  /**
   * @copydoc network::IRestRequestReceiver::Receive
   */
  void Receive(network::Uri local_uri,
               cpp::NnSp<network::IRestRequestHandler> handler) override;

 private:
  void HandleHttpRequest(const web::http::http_request &request) const;

  cpp::Sp<network::IRestRequestHandler> handler_;
  cpp::Up<web::http::experimental::listener::http_listener> http_listener_;
  cpp::NnSp<log::ILogger> logger_;
};
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_REST_REQUEST_RECEIVER_H_
