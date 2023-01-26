#ifndef STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SPS_STREAM_FACTORY_H_
#define STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SPS_STREAM_FACTORY_H_

#include <cppcoro/task.hpp>

#include "core_i_symbols_db.h"
#include "core_sps_price_recorder.h"
#include "core_types.h"
#include "cpp_factory.h"
#include "cpp_meta_thread_safe.h"
#include "cpp_not_null.h"
#include "network_i_ws_client.h"
#include "networkx_web_socket.h"

namespace stonks::core::sps {
/**
 * @brief Creates book tick web socket.
 */
class StreamFactory {
 public:
  StreamFactory(cpp::meta::ThreadSafe<cpp::NnUp<ISymbolsDb>> symbols_db,
                cpp::meta::ThreadSafe<cpp::Factory<network::IWsClient>>
                    ws_client_factory);

  /**
   * @brief Creates web socket from args.
   * @param symbol Symbol for which to get book ticks.
   */
  auto Create [[nodiscard]] (Symbol symbol)
  -> networkx::WebSocket<&PriceRecorder::RecordAsPrice>;

 private:
  cpp::NnSp<ISymbolsDb> symbols_db_;
  cpp::Factory<network::IWsClient> ws_client_factory_;
};
}  // namespace stonks::core::sps

#endif  // STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SPS_STREAM_FACTORY_H_
