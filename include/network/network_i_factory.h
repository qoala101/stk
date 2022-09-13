#ifndef STONKS_NETWORK_NETWORK_I_FACTORY_H_
#define STONKS_NETWORK_NETWORK_I_FACTORY_H_

#include "cpp_not_null.h"
#include "network_i_rest_request_receiver.h"
#include "network_i_rest_request_sender.h"

namespace stonks::network {
/**
 * @brief Factory of network interfaces to be provided by the implementation.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IFactory {
 public:
  virtual ~IFactory() = default;

  /**
   * @brief Creates executor of REST request.
   */
  [[nodiscard]] virtual auto CreateRestRequestSender() const
      -> cpp::NnUp<IRestRequestSender> = 0;

  /**
   * @brief Creates the receiver of REST requests on the specified URI.
   */
  [[nodiscard]] virtual auto CreateRestRequestReceiver() const
      -> cpp::NnUp<IRestRequestReceiver> = 0;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_I_FACTORY_H_
