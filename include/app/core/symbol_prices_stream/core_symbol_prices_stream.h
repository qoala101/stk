#ifndef STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SYMBOL_PRICES_STREAM_H_
#define STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SYMBOL_PRICES_STREAM_H_

#include "core_i_symbols_db.h"
#include "core_types.h"
#include "cpp_not_null.h"
#include "network_ws_client_builder.h"
#include "network_ws_connection.h"

namespace stonks::core {
/**
 * @brief Receives single symbol prices from Binance
 * and sends them to the Symbols DB.
 */
class SymbolPricesStream {
 public:
  SymbolPricesStream(Symbol symbol, network::WsClientBuilder ws_client_builder,
                     cpp::NnUp<ISymbolsDb> symbols_db);

 private:
  /**
   * @copydoc endpoints::BinanceSymbolBookTickerStream
   */
  static auto BinanceSymbolBookTickerStream
      [[nodiscard]] (Symbol symbol, cpp::NnUp<ISymbolsDb> symbols_db);

  network::WsConnection ws_connection_;
};
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SYMBOL_PRICES_STREAM_H_
