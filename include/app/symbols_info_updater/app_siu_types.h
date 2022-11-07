#ifndef STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_TYPES_H_
#define STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_TYPES_H_

#include <string>

#include "core_types.h"
#include "cpp_typed_struct.h"

namespace stonks::app::siu {
struct BinanceSymbolExchangeInfo {
  std::string symbol{};
  std::string base_asset{};
  std::string quote_asset{};
  std::string min_quantity{};
  std::string step_size{};
  std::string min_notional{};
  std::string tick_size{};
};

struct BinanceExchangeInfo
    : public cpp::TypedStruct<std::vector<BinanceSymbolExchangeInfo>> {};
}  // namespace stonks::app::siu

#endif  // STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_TYPES_H_
