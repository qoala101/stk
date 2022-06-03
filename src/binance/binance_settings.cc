#include "binance_settings.h"

#include <bits/exception.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <fstream>
#include <optional>
#include <string_view>

namespace stonks::binance::settings {
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

std::string GetBaseRestUri() {
  static const auto value =
      GetFirstWordFromFile("../settings/binance_base_rest_uri.txt");
  return value.value_or(std::string{});
}

std::string GetBaseStreamUri() {
  static const auto value =
      GetFirstWordFromFile("../settings/binance_base_stream_uri.txt");
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