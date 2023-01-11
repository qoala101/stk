#ifndef STONKS_RESTSDK_RESTSDK_REST_REQUEST_SENDER_H_
#define STONKS_RESTSDK_RESTSDK_REST_REQUEST_SENDER_H_

#include <cppcoro/task.hpp>

#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "network_i_rest_request_sender.h"
#include "network_types.h"

namespace web {
class uri;

namespace http::client {
class http_client;  // IWYU pragma: keep
}  // namespace http::client
}  // namespace web

namespace stonks::restsdk {
/**
 * @copydoc network::IRestRequestSender
 */
class RestRequestSender : public network::IRestRequestSender {
 public:
  explicit RestRequestSender(cpp::NnUp<log::ILogger> logger);

  RestRequestSender(const RestRequestSender &) = delete;
  RestRequestSender(RestRequestSender &&) noexcept;

  auto operator=(const RestRequestSender &) -> RestRequestSender & = delete;
  auto operator=(RestRequestSender &&) noexcept -> RestRequestSender &;

  ~RestRequestSender() override;

  /**
   * @copydoc network::IRestRequestSender::SendRequestAndGetResponse
   */
  auto SendRequestAndGetResponse [[nodiscard]] (network::RestRequest request)
  -> cppcoro::task<network::RestResponse> override;

 private:
  void ConnectClientTo(const web::uri &authority);

  cpp::Up<web::http::client::http_client> http_client_{};
  cpp::NnUp<log::ILogger> logger_;
};
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_REST_REQUEST_SENDER_H_
