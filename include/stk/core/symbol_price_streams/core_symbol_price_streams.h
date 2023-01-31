#ifndef VH_STK_CORE_SYMBOL_PRICE_STREAMS_H_
#define VH_STK_CORE_SYMBOL_PRICE_STREAMS_H_

#include <absl/time/time.h>

#include <vector>

#include "core_i_symbols_db.h"
#include "core_sps_stream_handle.h"
#include "core_types.h"
#include "cpp_factory.h"
#include "cpp_meta_thread_safe.h"
#include "cpp_not_null.h"
#include "network_i_ws_client.h"

namespace vh::stk::core {
/**
 * @brief Streams prices of specified symbols to Symbols DB.
 */
class SymbolPriceStreams {
 public:
  /**
   * @param symbols Symbols for which to receive the prices.
   * @param reattempt_interval Time in which to reattempt stream connection
   * if it fails.
   */
  SymbolPriceStreams(const std::vector<Symbol> &symbols,
                     absl::Duration reattempt_interval,
                     cpp::meta::ThreadSafe<cpp::NnUp<ISymbolsDb>> symbols_db,
                     cpp::meta::ThreadSafe<cpp::Factory<network::IWsClient>>
                         ws_client_factory);

 private:
  std::vector<sps::StreamHandle> stream_handles_{};
};
}  // namespace vh::stk::core

#endif  // VH_STK_CORE_SYMBOL_PRICE_STREAMS_H_
