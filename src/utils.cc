#include "utils.h"

#include <date/date.h>
#include <openssl/hmac.h>
#include <spdlog/spdlog.h>

#include <array>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <ctime>

namespace stonks::utils {
std::chrono::milliseconds GetUnixTime() {
  const auto current_time = std::chrono::system_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(
      current_time.time_since_epoch());
}

std::chrono::milliseconds ParseUnixTimeFromString(std::string_view time,
                                                  std::string_view format) {
  auto input_stream = std::istringstream{std::string{time}};
  auto parsed_time = std::chrono::time_point<std::chrono::system_clock,
                                             std::chrono::seconds>{};
  input_stream >> date::parse(std::string{format}, parsed_time);
  return std::chrono::duration_cast<std::chrono::milliseconds>(
      parsed_time.time_since_epoch());
}

std::string ConvertUnixTimeToString(std::chrono::milliseconds time,
                                    std::string_view format) {
  const auto epoch_start_time_point =
      std::chrono::system_clock::from_time_t(std::time_t{0});
  const auto time_point = epoch_start_time_point + time;
  return date::format(std::string{format}, time_point);
}

std::string SignUsingHmacSha256(std::string_view data, std::string_view key) {
  const auto raw_key = static_cast<const void *>(key.data());
  const auto raw_data = reinterpret_cast<const unsigned char *>(data.data());

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

boost::uuids::uuid ParseUuidFromString(std::string_view uuid) {
  return boost::lexical_cast<boost::uuids::uuid>(uuid);
}

std::string ConvertUuidToString(boost::uuids::uuid uuid) {
  return boost::lexical_cast<std::string>(uuid);
}
}  // namespace stonks::utils