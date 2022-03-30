#ifndef STONKS_UTILS_H_
#define STONKS_UTILS_H_

#include <string>

namespace stonks::utils {
int64_t GetUnixTimeMillis();

std::string SignUsingHmacSha256(std::string_view data, std::string_view key);
}  // namespace stonks::utils

#endif  // STONKS_UTILS_H_
