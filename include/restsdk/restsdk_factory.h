#ifndef STONKS_RESTSDK_RESTSDK_FACTORY_H_
#define STONKS_RESTSDK_RESTSDK_FACTORY_H_

#include <polymorphic_value.h>

#include <memory>
#include <string>

#include "network_i_factory.h"
#include "network_i_json.h"
#include "network_i_rest_request_handler.h"
#include "network_i_rest_request_receiver.h"
#include "network_i_rest_request_sender.h"
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
      std::string local_uri,
      cpp::not_null<std::unique_ptr<network::IRestRequestHandler>> handler)
      const
      -> cpp::not_null<std::unique_ptr<network::IRestRequestReceiver>> override;

  /**
   * @copydoc network::IFactory::CreateJson
   */
  [[nodiscard]] auto CreateJson() const
      -> isocpp_p0201::polymorphic_value<network::IJson> override;
};
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_FACTORY_H_
