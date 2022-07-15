#ifndef STONKS_NETWORK_NETWORK_FACTORY_H_
#define STONKS_NETWORK_NETWORK_FACTORY_H_

#include <polymorphic_value.h>

#include <vector>

#include "network_json.h"
#include "network_rest_request_receiver.h"
#include "network_rest_request_sender.h"
#include "network_types.h"
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
   * @param endpoint Request destination.
   */
  [[nodiscard]] virtual auto CreateRestRequestSender(Endpoint endpoint) const
      -> cpp::not_null<std::unique_ptr<IRestRequestSender>> = 0;

  /**
   * @brief Creates the handler of REST requests on the specified URI.
   * @param local_uri Request handler would listen to REST requests which come
   * to this URI.
   * @param handler Function to be called when request is received.
   */
  [[nodiscard]] virtual auto CreateRestRequestReceiver(
      std::string_view local_uri,
      std::function<std::pair<Status, Result>(Endpoint, RestRequestData)>
          handler) const
      -> cpp::not_null<std::unique_ptr<IRestRequestReceiver>> = 0;

  /**
   * @brief Creates an empty JSON object.
   */
  [[nodiscard]] virtual auto CreateJson() const
      -> isocpp_p0201::polymorphic_value<IJson> = 0;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_FACTORY_H_
