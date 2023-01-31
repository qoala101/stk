#ifndef VH_STK_CORE_SPS_STREAM_FACTORY_H_
#define VH_STK_CORE_SPS_STREAM_FACTORY_H_

#include <cppcoro/task.hpp>

#include "core_i_symbols_db.h"
#include "core_sps_price_recorder.h"
#include "core_types.h"
#include "cpp_factory.h"
#include "cpp_not_null.h"
#include "network_i_ws_client.h"
#include "networkx_web_socket.h"

namespace vh::stk::core::sps {
/**
 * @brief Creates book tick web socket.
 */
class StreamFactory {
 public:
  StreamFactory(cpp::NnSp<ISymbolsDb> symbols_db,
                cpp::NnSp<cpp::Factory<network::IWsClient>> ws_client_factory);

  /**
   * @brief Creates web socket from args.
   * @param symbol Symbol for which to get book ticks.
   */
  auto Create [[nodiscard]] (Symbol symbol)
  -> networkx::WebSocket<&PriceRecorder::RecordAsPrice>;

 private:
  cpp::NnSp<ISymbolsDb> symbols_db_;
  cpp::NnSp<cpp::Factory<network::IWsClient>> ws_client_factory_;
};
}  // namespace vh::stk::core::sps

#endif  // VH_STK_CORE_SPS_STREAM_FACTORY_H_
