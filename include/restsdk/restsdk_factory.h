#ifndef STONKS_RESTSDK_RESTSDK_FACTORY_H_
#define STONKS_RESTSDK_RESTSDK_FACTORY_H_

#include <polymorphic_value.h>

#include <memory>
#include <string_view>

#include "network_i_factory.h"
#include "network_i_json.h"
#include "network_i_rest_request_receiver.h"
#include "network_i_rest_request_sender.h"
#include "network_types.h"
#include "not_null.hpp"

namespace stonks::restsdk {
/**
 * @copydoc network::IFactory
 */
class Factory : public network::IFactory {
 public:
  /**
   * @copydoc network::IFactory::CreateRestRequestSender
   */
  [[nodiscard]] auto CreateRestRequestSender() const
      -> cpp::not_null<std::unique_ptr<network::IRestRequestSender>> override;

  /**
   * @copydoc network::IFactory::CreateRestRequestReceiver
   */
  [[nodiscard]] auto CreateRestRequestReceiver(
      std::string_view local_uri,
      const network::RestRequestHandler &handler) const
      -> cpp::not_null<std::unique_ptr<network::IRestRequestReceiver>> override;

  /**
   * @copydoc network::IFactory::CreateJson
   */
  [[nodiscard]] auto CreateJson() const
      -> isocpp_p0201::polymorphic_value<network::IJson> override;
};
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_FACTORY_H_
