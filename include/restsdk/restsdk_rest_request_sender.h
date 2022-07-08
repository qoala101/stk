#ifndef STONKS_RESTSDK_RESTSDK_REST_REQUEST_SENDER_H_
#define STONKS_RESTSDK_RESTSDK_REST_REQUEST_SENDER_H_

#include "network_json.h"
#include "network_rest_request_sender.h"
#include "network_types.h"

namespace stonks::restsdk {
/**
 * @copydoc network::IRestRequestSender
 */
class RestRequestSender : public network::IRestRequestSender {
 public:
  /**
   * @param endpoint Request destination.
   */
  explicit RestRequestSender(network::Endpoint endpoint);

  /**
   * @copydoc network::IRestRequestSender::SendRequestAndGetResponse
   */
  // NOLINTNEXTLINE(*-use-nodiscard)
  auto SendRequestAndGetResponse(const network::RestRequestData &data) const
      -> network::Json override;

 private:
  network::Endpoint endpoint_{};
};
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_REST_REQUEST_SENDER_H_
