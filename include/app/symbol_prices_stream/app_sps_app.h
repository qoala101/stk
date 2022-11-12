#ifndef STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_APP_H_
#define STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_APP_H_

#include "core_types.h"
#include "cpp_not_null.h"
#include "network_ws_client_builder.h"
#include "network_ws_connection.h"
#include "symbols_db/app_sdb_i_app.h"

namespace stonks::app::sps {
/**
 * @brief Receives single symbol prices from Binance
 * and sends them to the Symbols DB.
 */
class App {
 public:
  App(core::Symbol symbol, network::WsClientBuilder ws_client_builder,
      cpp::NnUp<sdb::IApp> sdb_app);

 private:
  /**
   * @copydoc endpoints::BinanceSymbolBookTickerStream
   */
  static auto BinanceSymbolBookTickerStream
      [[nodiscard]] (core::Symbol symbol, cpp::NnUp<sdb::IApp> sdb_app);

  network::WsConnection ws_connection_;
};
}  // namespace stonks::app::sps

#endif  // STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_APP_H_
