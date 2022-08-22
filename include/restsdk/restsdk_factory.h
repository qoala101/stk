#ifndef STONKS_RESTSDK_RESTSDK_FACTORY_H_
#define STONKS_RESTSDK_RESTSDK_FACTORY_H_

#include <string>

#include "cpp_not_null.h"
#include "cpp_polymorphic_value.h"
#include "network_i_factory.h"
#include "network_i_json.h"
#include "network_i_rest_request_handler.h"
#include "network_i_rest_request_receiver.h"
#include "network_i_rest_request_sender.h"

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
      -> cpp::NnUp<network::IRestRequestSender> override;

  /**
   * @copydoc network::IFactory::CreateRestRequestReceiver
   */
  [[nodiscard]] auto CreateRestRequestReceiver(
      std::string local_uri,
      cpp::NnUp<network::IRestRequestHandler> handler) const
      -> cpp::NnUp<network::IRestRequestReceiver> override;

  /**
   * @copydoc network::IFactory::CreateJson
   */
  [[nodiscard]] auto CreateJson() const -> cpp::Pv<network::IJson> override;
};
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_FACTORY_H_
