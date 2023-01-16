#ifndef STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SYMBOL_PRICE_STREAMS_H_
#define STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SYMBOL_PRICE_STREAMS_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>
#include <vector>

#include "core_sps_stream_factory.h"
#include "core_sps_stream_handle.h"
#include "core_types.h"

namespace stonks::core {
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
  SymbolPriceStreams(std::vector<Symbol> symbols,
                     absl::Duration reattempt_interval,
                     sps::StreamFactory stream_factory);

  /**
   * @brief Gives symbols for which prices are streamed.
   */
  auto GetStreamedSymbols [[nodiscard]] () const
      -> cppcoro::task<std::vector<Symbol>>;

 private:
  std::vector<Symbol> symbols_{};
  std::vector<sps::StreamHandle> stream_handles_{};
};
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_SYMBOL_PRICE_STREAMS_CORE_SYMBOL_PRICE_STREAMS_H_
