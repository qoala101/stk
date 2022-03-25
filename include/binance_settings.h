#ifndef STONKS_BINANCE_SETTINGS_H_
#define STONKS_BINANCE_SETTINGS_H_

#include <string>

namespace stonks::binance::settings {
std::string BaseUri();

std::string ApiKey();

std::string SecretKey();
}  // namespace stonks::binance::settings

#endif  // STONKS_BINANCE_SETTINGS_H_
