#include "binance_settings.h"

#include <fstream>

#include <spdlog/spdlog.h>

namespace
{
  std::optional<std::string> FirstWordFromFile(const std::string &file_path)
  {
    auto file_stream = std::ifstream{file_path};

    if (!file_stream.good())
    {
      spdlog::error("Cannot read file {}", file_path);
      return std::nullopt;
    }

    auto file_content = std::string{};
    file_stream >> file_content;
    return file_content;
  }
}

namespace stonks::binance::settings
{
  std::string BaseUri()
  {
    static const auto value_opt = FirstWordFromFile("../binance_base_uri.txt");
    return value_opt.value_or(std::string{});
  }

  std::string ApiKey()
  {
    static const auto value_opt = FirstWordFromFile("../binance_api_key.txt");
    return value_opt.value_or(std::string{});
  }

  std::string SecretKey()
  {
    static const auto value_opt = FirstWordFromFile("../binance_secret_key.txt");
    return value_opt.value_or(std::string{});
  }
}