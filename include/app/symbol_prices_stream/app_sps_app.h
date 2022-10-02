#ifndef STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_APP_H_
#define STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_APP_H_

#include "app_sps_sdb_app_client.h"
#include "core_types.h"
#include "cpp_not_null.h"
#include "network_aprh_ws_handler_variant.h"
#include "network_i_ws_client.h"
#include "network_ws_connection.h"

namespace stonks::app::sps {
/**
 * @brief Receives single symbol prices from Binance
 * and sends them to the Symbols DB.
 */
class App {
 public:
  App(core::Symbol symbol, SdbAppClient sdb_app_client,
      cpp::NnUp<network::IWsClient> ws_client);

 private:
  /**
   * @copydoc endpoints::BinanceSymbolBookTickerStream
   */
  [[nodiscard]] static auto BinanceSymbolBookTickerStream(
      core::Symbol symbol, SdbAppClient sdb_app_client)
      -> network::aprh::HandlerWithWsMessage;

  network::WsConnection ws_connection_;
};
}  // namespace stonks::app::sps

#endif  // STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_APP_H_
