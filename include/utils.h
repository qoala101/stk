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

template <typename T>
constexpr std::chrono::milliseconds FloorTo(std::chrono::milliseconds time) {
  constexpr auto coef = T::period::num * 1000;
  return std::chrono::milliseconds{(time.count() / coef) * coef};
}

/**
 * @return Unix GMT milliseconds or nullopt if cannot parse.
 */
std::chrono::milliseconds ParseUnixTimeFromString(
    std::string_view time, std::string_view format = "%d %b %Y %H:%M:%S");

/**
 * @remark %OS produces whole number of seconds whereas %S would produce seconds
 * with decimals.
 * @return Unix GMT milliseconds or nullopt if cannot parse.
 */
std::string ConvertUnixTimeToString(
    std::chrono::milliseconds time,
    std::string_view format = "%d %b %Y %H:%M:%OS");

std::string SignUsingHmacSha256(std::string_view data, std::string_view key);
}  // namespace stonks::utils

#endif  // STONKS_UTILS_H_
