/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_RESTSDK_REST_REQUEST_SENDER_H_
#define VH_RESTSDK_REST_REQUEST_SENDER_H_

// IWYU pragma: no_include <cpprest/http_client.h>

#include <cppcoro/task.hpp>

#include "cpp_mutex.h"
#include "cpp_not_null.h"
#include "cpp_smart_pointers.h"
#include "log_i_logger.h"
#include "network_i_rest_request_sender.h"
#include "network_types.h"

namespace web {
class uri;

namespace http::client {
class http_client;  // IWYU pragma: keep
}  // namespace http::client
}  // namespace web

namespace vh::restsdk {
/**
 * @copydoc network::IRestRequestSender
 * @param http_client_mutex Protects lifetime of the native http client.
 */
class RestRequestSender : public network::IRestRequestSender {
 public:
  RestRequestSender(cpp::NnUp<log::ILogger> logger,
                    cpp::MutexVariant http_client_mutex);

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
  cpp::MutexVariant http_client_mutex_{};
};
}  // namespace vh::restsdk

#endif  // VH_RESTSDK_REST_REQUEST_SENDER_H_
