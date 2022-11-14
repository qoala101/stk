#ifndef STONKS_APP_CORE_SYMBOLS_INFO_UPDATER_CORE_SIU_TYPES_H_
#define STONKS_APP_CORE_SYMBOLS_INFO_UPDATER_CORE_SIU_TYPES_H_

#include <string>

#include "cpp_typed_struct.h"

namespace stonks::core::siu {
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
}  // namespace stonks::core::siu

#endif  // STONKS_APP_CORE_SYMBOLS_INFO_UPDATER_CORE_SIU_TYPES_H_
