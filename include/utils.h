#ifndef STONKS_UTILS_H_
#define STONKS_UTILS_H_

#include <string>

namespace stonks::utils
{
  long UnixTimeMillis();

  std::string UnixTimeMillisAsString();

  std::string SignUsingHmacSha256(const std::string &data, const std::string &key);
}

#endif // STONKS_UTILS_H_
