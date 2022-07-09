#ifndef STONKS_RESTSDK_RESTSDK_FACTORY_H_
#define STONKS_RESTSDK_RESTSDK_FACTORY_H_

#include <memory>
#include <string_view>

#include "network_factory.h"
#include "network_json.h"
#include "network_rest_request_handler.h"
#include "network_rest_request_sender.h"
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
  [[nodiscard]] auto CreateRestRequestSender(network::Endpoint endpoint) const
      -> cpp::not_null<std::unique_ptr<network::IRestRequestSender>> override;

  /**
   * @copydoc network::IFactory::CreateRestRequestHandler
   */
  [[nodiscard]] auto CreateRestRequestHandler(std::string_view local_uri) const
      -> cpp::not_null<std::unique_ptr<network::IRestRequestHandler>> override;

  /**
   * @copydoc network::IFactory::CreateJson
   */
  [[nodiscard]] auto CreateJson() const
      -> cpp::not_null<std::unique_ptr<network::IJson>> override;
};
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_FACTORY_H_
