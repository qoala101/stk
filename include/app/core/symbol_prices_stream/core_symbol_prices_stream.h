#ifndef STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SYMBOL_PRICES_STREAM_H_
#define STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SYMBOL_PRICES_STREAM_H_

#include "core_i_symbols_db.h"
#include "core_sps_book_tick_handler.h"
#include "core_types.h"
#include "cpp_not_null.h"
#include "network_i_ws_client.h"
#include "networkx_web_socket.h"

namespace stonks::core {
/**
 * @brief While alive, receives single symbol prices from Binance book ticks
 * web socket and records them to Symbols DB.
 */
class SymbolPricesStream {
 public:
  SymbolPricesStream(Symbol symbol, cpp::NnUp<ISymbolsDb> symbols_db,
                     cpp::NnUp<network::IWsClient> ws_client);

 private:
  networkx::WebSocket<&sps::BookTickHandler::RecordAsPrice> web_socket_;
};
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SYMBOL_PRICES_STREAM_H_
