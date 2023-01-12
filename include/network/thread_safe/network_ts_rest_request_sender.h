#ifndef STONKS_NETWORK_THREAD_SAFE_NETWORK_TS_REST_REQUEST_SENDER_H_
#define STONKS_NETWORK_THREAD_SAFE_NETWORK_TS_REST_REQUEST_SENDER_H_

#include <cppcoro/task.hpp>
#include <mutex>
#include <utility>

#include "network_i_rest_request_sender.h"
#include "network_types.h"

namespace stonks::network::ts {
/**
 * @brief Request sender wrapper which adds synchronization layer.
 */
class RestRequestSender : public IRestRequestSender {
 public:
  /**
   * @param request_sender Sender to wrap.
   */
  explicit RestRequestSender(cpp::NnSp<IRestRequestSender> request_sender);

  /**
   * @copydoc IRestRequestSender::SendRequestAndGetResponse
   */
  auto SendRequestAndGetResponse [[nodiscard]] (RestRequest request)
  -> cppcoro::task<RestResponse> override;

 private:
  cpp::NnSp<IRestRequestSender> request_sender_;
  std::mutex mutex_{};
};
}  // namespace stonks::network::ts

#endif  // STONKS_NETWORK_THREAD_SAFE_NETWORK_TS_REST_REQUEST_SENDER_H_
