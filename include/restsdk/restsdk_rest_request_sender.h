#ifndef STONKS_RESTSDK_RESTSDK_REST_REQUEST_SENDER_H_
#define STONKS_RESTSDK_RESTSDK_REST_REQUEST_SENDER_H_

#include <cppcoro/task.hpp>

#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "network_i_rest_request_sender.h"
#include "network_types.h"

namespace stonks::restsdk {
/**
 * @copydoc network::IRestRequestSender
 */
class RestRequestSender : public network::IRestRequestSender {
 public:
  explicit RestRequestSender(cpp::NnUp<log::ILogger> logger);

  /**
   * @copydoc network::IRestRequestSender::SendRequestAndGetResponse
   */
  auto SendRequestAndGetResponse
      [[nodiscard]] (network::RestRequest request) const
      -> cppcoro::task<network::RestResponse> override;

 private:
  cpp::NnUp<log::ILogger> logger_;
};
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_REST_REQUEST_SENDER_H_
