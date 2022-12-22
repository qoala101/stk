#ifndef STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SPS_BOOK_TICK_WEB_SOCKET_FACTORY_H_
#define STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SPS_BOOK_TICK_WEB_SOCKET_FACTORY_H_

#include <cppcoro/task.hpp>

#include "core_i_symbol_prices_streams_controller.h"
#include "core_i_symbols_db.h"
#include "core_sps_book_tick_handler.h"
#include "core_types.h"
#include "di_factory.h"
#include "network_i_ws_client.h"
#include "network_ws_types.h"
#include "networkx_web_socket.h"

namespace stonks::core::sps {
/**
 * @brief Creates book tick web socket.
 */
class BookTickWebSocketFactory {
 public:
  /**
   * @param controller Provides the symbol for which to get book ticks.
   */
  BookTickWebSocketFactory(cpp::NnUp<ISymbolPricesStreamsController> controller,
                           di::Factory<ISymbolsDb> symbols_db_factory,
                           di::Factory<network::IWsClient> ws_client_factory);

  /**
   * @brief Creates web socket from args.
   */
  auto Create [[nodiscard]] () const
      -> cppcoro::task<networkx::WebSocket<&BookTickHandler::RecordAsPrice>>;

 private:
  cpp::NnUp<ISymbolPricesStreamsController> controller_;
  di::Factory<ISymbolsDb> symbols_db_factory_;
  di::Factory<network::IWsClient> ws_client_factory_;
};
}  // namespace stonks::core::sps

#endif  // STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SPS_BOOK_TICK_WEB_SOCKET_FACTORY_H_
