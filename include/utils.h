#ifndef STONKS_UTILS_H_
#define STONKS_UTILS_H_

#include <optional>
#include <string>

namespace stonks::utils {
/**
 * @return Unix GMT milliseconds.
 */
int64_t GetUnixTimeMillis();

/**
 * @param time Date time in the format "9 Jan 2014 12:35:34".
 * @return Unix GMT milliseconds or nullopt if cannot parse.
 */
std::optional<int64_t> GetUnixTimeMillisFromString(std::string_view time);

std::string SignUsingHmacSha256(std::string_view data, std::string_view key);
}  // namespace stonks::utils

#endif  // STONKS_UTILS_H_
