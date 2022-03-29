#include "binance_settings.h"

#include <spdlog/spdlog.h>

#include <fstream>

namespace {
std::optional<std::string> GetFirstWordFromFile(std::string_view file_path) {
  auto file_stream = std::ifstream{std::string{file_path}};

  if (!file_stream.good()) {
    spdlog::error("Cannot read file {}", file_path);
    return std::nullopt;
  }

  auto file_content = std::string{};
  file_stream >> file_content;
  return file_content;
}
}  // namespace

namespace stonks::binance::settings {
std::string GetBaseUri() {
  static const auto value =
      GetFirstWordFromFile("../settings/binance_base_uri.txt");
  return value.value_or(std::string{});
}

std::string GetApiKey() {
  static const auto value =
      GetFirstWordFromFile("../settings/binance_api_key.txt");
  return value.value_or(std::string{});
}

std::string GetSecretKey() {
  static const auto value =
      GetFirstWordFromFile("../settings/binance_secret_key.txt");
  return value.value_or(std::string{});
}
}  // namespace stonks::binance::settings