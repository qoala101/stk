#include "utils.h"

#include <openssl/hmac.h>
#include <spdlog/spdlog.h>

#include <array>
#include <chrono>

namespace stonks::utils {
long UnixTimeMillis() {
  const auto current_time = std::chrono::system_clock::now().time_since_epoch();
  return long{
      std::chrono::duration_cast<std::chrono::milliseconds>(current_time)
          .count()};
}

std::string UnixTimeMillisAsString() {
  return std::to_string(UnixTimeMillis());
}

std::string SignUsingHmacSha256(const std::string &data,
                                const std::string &key) {
  const auto raw_key = static_cast<const void *>(key.c_str());
  const auto raw_data = reinterpret_cast<const unsigned char *>(data.c_str());

  static const auto kSignedDataSize = 32;

  auto signed_data_array = std::array<unsigned char, kSignedDataSize>{};
  auto signed_data_length = (unsigned int){};

  HMAC(EVP_sha256(), raw_key, key.length(), raw_data, data.length(),
       signed_data_array.data(), &signed_data_length);

  static const auto kDecodedSignedDataSize = 64;

  auto decoded_signed_data_array = std::array<char, kDecodedSignedDataSize>{};

  for (auto i = 0; i < signed_data_length; ++i) {
    sprintf(&decoded_signed_data_array[i * 2], "%02hhx", signed_data_array[i]);
  }

  return std::string{decoded_signed_data_array.data(), kDecodedSignedDataSize};
}
}  // namespace stonks::utils