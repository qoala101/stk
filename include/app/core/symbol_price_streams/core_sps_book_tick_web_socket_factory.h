#ifndef STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SPS_BOOK_TICK_WEB_SOCKET_FACTORY_H_
#define STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SPS_BOOK_TICK_WEB_SOCKET_FACTORY_H_

#include <cppcoro/task.hpp>

#include "common_thread_safe_tag.h"
#include "core_i_symbols_db.h"
#include "core_i_symbols_db_updater.h"
#include "core_sps_book_tick_handler.h"
#include "core_types.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "di_factory.h"
#include "network_i_ws_client.h"
#include "networkx_web_socket.h"

namespace stonks::core::sps {
/**
 * @brief Creates book tick web socket.
 */
class BookTickWebSocketFactory {
 public:
  BookTickWebSocketFactory(
      common::ThreadSafe<cpp::NnUp<ISymbolsDb>> symbols_db,
      common::ThreadSafe<cpp::NnUp<ISymbolsDbUpdater>> symbols_db_updater,
      common::ThreadSafe<di::Factory<network::IWsClient>> ws_client_factory);

  /**
   * @brief Creates web socket from args.
   * @param symbol Symbol for which to get book ticks.
   */
  auto Create [[nodiscard]] (Symbol symbol) const
      -> cppcoro::task<networkx::WebSocket<&BookTickHandler::RecordAsPrice>>;

 private:
  auto GetLastPriceRecord [[nodiscard]] (const Symbol &symbol) const
      -> cppcoro::task<cpp::Opt<SymbolPriceRecord>>;

  cpp::NnSp<ISymbolsDb> symbols_db_;
  cpp::NnUp<ISymbolsDbUpdater> symbols_db_updater_;
  di::Factory<network::IWsClient> ws_client_factory_;
};
}  // namespace stonks::core::sps

#endif  // STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SPS_BOOK_TICK_WEB_SOCKET_FACTORY_H_
