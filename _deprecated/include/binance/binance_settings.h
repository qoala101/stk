#ifndef STONKS_BINANCE_BINANCE_SETTINGS_H_
#define STONKS_BINANCE_BINANCE_SETTINGS_H_

#include <string>

namespace stonks::binance::settings {
std::string GetBaseRestUri();

std::string GetBaseStreamUri();

std::string GetApiKey();

std::string GetSecretKey();
}  // namespace stonks::binance::settings

#endif  // STONKS_BINANCE_BINANCE_SETTINGS_H_
