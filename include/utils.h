#ifndef STONKS_UTILS_H_
#define STONKS_UTILS_H_

#include <chrono>
#include <optional>
#include <string>

namespace stonks::utils {
/**
 * @return Unix GMT milliseconds.
 */
std::chrono::milliseconds GetUnixTime();

/**
 * @return Unix GMT milliseconds or nullopt if cannot parse.
 */
std::optional<std::chrono::milliseconds> GetUnixTimeFromString(
    std::string_view time, std::string_view format = "%d %b %Y %H:%M:%S");

std::string SignUsingHmacSha256(std::string_view data, std::string_view key);
}  // namespace stonks::utils

#endif  // STONKS_UTILS_H_
