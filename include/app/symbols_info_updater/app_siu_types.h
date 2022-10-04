#ifndef STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_TYPES_H_
#define STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_TYPES_H_

#include "core_types.h"
#include "cpp_typed_struct.h"

namespace stonks::app::siu {
struct BinanceSymbolExchangeInfo {
  core::Symbol symbol{};
  core::Asset base_asset{};
  core::Asset quote_asset{};
  double min_quantity{};
  double step_size{};
  double min_notional{};
  double tick_size{};
};
}  // namespace stonks::app::siu

#endif  // STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_TYPES_H_
