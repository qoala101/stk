#ifndef STONKS_NETWORK_NETWORK_FACTORY_H_
#define STONKS_NETWORK_NETWORK_FACTORY_H_

#include "network_rest_request_sender.h"
#include "not_null.hpp"

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
      -> cpp::not_null<std::unique_ptr<IRestRequestSender>> = 0;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_FACTORY_H_
