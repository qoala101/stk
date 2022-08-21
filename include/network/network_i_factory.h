#ifndef STONKS_NETWORK_NETWORK_I_FACTORY_H_
#define STONKS_NETWORK_NETWORK_I_FACTORY_H_

#include <polymorphic_value.h>

#include <vector>

#include "network_i_json.h"
#include "network_i_rest_request_handler.h"
#include "network_i_rest_request_receiver.h"
#include "network_i_rest_request_sender.h"
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
   */
  [[nodiscard]] virtual auto CreateRestRequestSender() const
      -> ccutils::NnUp<IRestRequestSender> = 0;

  /**
   * @brief Creates the handler of REST requests on the specified URI.
   * @param local_uri Request handler would listen to REST requests which come
   * to this URI.
   * @param handler Function to be called when request is received.
   */
  [[nodiscard]] virtual auto CreateRestRequestReceiver(
      std::string local_uri, ccutils::NnUp<IRestRequestHandler> handler) const
      -> ccutils::NnUp<IRestRequestReceiver> = 0;

  /**
   * @brief Creates an empty JSON object.
   */
  [[nodiscard]] virtual auto CreateJson() const -> ccutils::Pv<IJson> = 0;

 protected:
  explicit IFactory() = default;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_I_FACTORY_H_
