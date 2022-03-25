#ifndef STONKS_BINANCE_SETTINGS_H_
#define STONKS_BINANCE_SETTINGS_H_

#include <string>

namespace stonks::binance::settings
{
  std::string BaseUri();

  std::string ApiKey();

  std::string SecretKey();
}

#endif // STONKS_BINANCE_SETTINGS_H_
