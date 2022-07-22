#ifndef STONKS_RESTSDK_RESTSDK_REST_REQUEST_RECEIVER_H_
#define STONKS_RESTSDK_RESTSDK_REST_REQUEST_RECEIVER_H_

#include <memory>
#include <string_view>

#include "network_i_rest_request_receiver.h"
#include "network_types.h"
#include "not_null.hpp"

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
                               network::RestRequestHandler handler);

  RestRequestReceiver(const RestRequestReceiver &other) = delete;
  RestRequestReceiver(RestRequestReceiver &&) noexcept;

  auto operator=(const RestRequestReceiver &other)
      -> RestRequestReceiver & = delete;
  auto operator=(RestRequestReceiver &&) noexcept -> RestRequestReceiver &;

  ~RestRequestReceiver() override;

 private:
  void HandleHttpRequest(const web::http::http_request &request) const;

  network::RestRequestHandler handler_{};
  cpp::not_null<
      std::unique_ptr<web::http::experimental::listener::http_listener>>
      http_listener_;
};
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_REST_REQUEST_RECEIVER_H_
