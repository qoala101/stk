#ifndef STONKS_RESTSDK_RESTSDK_REST_REQUEST_SENDER_H_
#define STONKS_RESTSDK_RESTSDK_REST_REQUEST_SENDER_H_

#include <memory>

#include "network_json.h"
#include "network_rest_request_sender.h"
#include "network_types.h"
#include "not_null.hpp"

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
      -> cpp::not_null<std::unique_ptr<network::IJson>> override;

 private:
  network::Endpoint endpoint_{};
};
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_REST_REQUEST_SENDER_H_
