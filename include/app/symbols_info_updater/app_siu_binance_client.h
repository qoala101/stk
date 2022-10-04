#ifndef STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_BINANCE_CLIENT_H_
#define STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_BINANCE_CLIENT_H_

#include <vector>

#include "app_siu_types.h"
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
  [[nodiscard]] auto BinanceExchangeInfo() const
      -> std::vector<BinanceSymbolExchangeInfo>;

 private:
  network::RestClient rest_client_;
};
}  // namespace stonks::app::siu

#endif  // STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_BINANCE_CLIENT_H_
