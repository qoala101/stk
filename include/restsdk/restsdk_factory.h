#ifndef STONKS_RESTSDK_RESTSDK_FACTORY_H_
#define STONKS_RESTSDK_RESTSDK_FACTORY_H_

#include "cpp_not_null.h"
#include "network_i_factory.h"
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
  [[nodiscard]] auto CreateRestRequestReceiver() const
      -> cpp::NnUp<network::IRestRequestReceiver> override;
};
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_FACTORY_H_
