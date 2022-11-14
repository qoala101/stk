#ifndef STONKS_APP_CORE_SYMBOLS_INFO_UPDATER_CORE_SIU_BINANCE_CLIENT_H_
#define STONKS_APP_CORE_SYMBOLS_INFO_UPDATER_CORE_SIU_BINANCE_CLIENT_H_

#include "core_siu_types.h"
#include "network_rest_client.h"

namespace stonks::app::siu {
/**
 * @brief Client to Binance REST API.
 */
class BinanceClient {
 public:
  explicit BinanceClient(network::RestClient rest_client);

  /**
   * @copydoc endpoints::BinanceExchangeInfo
   */
  auto BinanceExchangeInfo [[nodiscard]] () const -> BinanceExchangeInfo;

 private:
  network::RestClient rest_client_;
};
}  // namespace stonks::app::siu

#endif  // STONKS_APP_CORE_SYMBOLS_INFO_UPDATER_CORE_SIU_BINANCE_CLIENT_H_
