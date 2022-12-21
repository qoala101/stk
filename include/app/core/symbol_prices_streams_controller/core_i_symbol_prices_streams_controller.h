#ifndef STONKS_APP_CORE_SYMBOL_PRICES_STREAMS_CONTROLLER_CORE_I_SYMBOL_PRICES_STREAMS_CONTROLLER_H_
#define STONKS_APP_CORE_SYMBOL_PRICES_STREAMS_CONTROLLER_CORE_I_SYMBOL_PRICES_STREAMS_CONTROLLER_H_

#include <cppcoro/task.hpp>

#include "core_types.h"

namespace stonks::core {
/**
 * @brief Controls instances of symbol price streams.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class ISymbolPricesStreamsController {
 public:
  virtual ~ISymbolPricesStreamsController() = default;

  /**
   * @brief Gives stream a symbol for which it should get prices.
   */
  virtual auto GetSymbolForStream [[nodiscard]] () const
      -> cppcoro::task<Symbol> = 0;
};
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_SYMBOL_PRICES_STREAMS_CONTROLLER_CORE_I_SYMBOL_PRICES_STREAMS_CONTROLLER_H_
