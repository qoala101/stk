#ifndef STONKS_RESTSDK_RESTSDK_REST_REQUEST_SENDER_H_
#define STONKS_RESTSDK_RESTSDK_REST_REQUEST_SENDER_H_

#include "log_i_logger.h"
#include "network_i_rest_request_sender.h"
#include "network_types.h"
#include "cpp_not_null.h"

namespace stonks::restsdk {
/**
 * @copydoc network::IRestRequestSender
 */
class RestRequestSender : public network::IRestRequestSender {
 public:
  explicit RestRequestSender(cpp::NnSp<log::ILogger> logger);

  /**
   * @copydoc network::IRestRequestSender::SendRequestAndGetResponse
   */
  [[nodiscard]] auto SendRequestAndGetResponse(
      network::RestRequest request) const -> network::RestResponse override;

 private:
  cpp::NnSp<log::ILogger> logger_;
};
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_REST_REQUEST_SENDER_H_
