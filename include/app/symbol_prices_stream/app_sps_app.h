#ifndef STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_APP_H_
#define STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_APP_H_

#include "app_sps_sdb_app_client.h"
#include "core_types.h"
#include "network_ws_client_builder.h"
#include "network_ws_connection.h"

namespace stonks::app::sps {
/**
 * @brief Receives single symbol prices from Binance
 * and sends them to the Symbols DB.
 */
class App {
 public:
  App(core::Symbol symbol, network::WsClientBuilder ws_client_builder,
      SdbAppClient sdb_app_client);

 private:
  /**
   * @copydoc endpoints::BinanceSymbolBookTickerStream
   */
  [[nodiscard]] static auto BinanceSymbolBookTickerStream(
      core::Symbol symbol, SdbAppClient sdb_app_client);

  network::WsConnection ws_connection_;
};
}  // namespace stonks::app::sps

#endif  // STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_APP_H_
