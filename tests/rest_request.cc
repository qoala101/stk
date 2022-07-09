#include <cpprest/json.h>
#include <gtest/gtest.h>

#include <magic_enum.hpp>
#include <memory>

#include "network_rest_request_builder.h"
#include "restsdk/restsdk_rest_request_sender.h"
#include "restsdk_json_impl.h"

TEST(RestRequest, AppendUri) {
  const auto [endpoint, data] = stonks::network::RestRequestBuilder{}
                                    .WithBaseUri("base_uri")
                                    .AppendUri("appended_uri")
                                    .Build();
  EXPECT_EQ(endpoint.uri, "base_uri/appended_uri");
}

namespace {
enum class DefaultNameEnum { kDefaultEnumName };
enum class CustomNameEnum { kCustomEnumName };
}  // namespace

template <>
constexpr auto magic_enum::customize::enum_name<CustomNameEnum>(
    CustomNameEnum value) noexcept -> std::string_view {
  switch (value) {
    case CustomNameEnum::kCustomEnumName:
      return "CUSTOM_ENUM_NAME";
    default:
      break;
  }

  return {};
}

TEST(RestRequest, ParameterTypesToString) {
  const auto [endpoint, data] =
      stonks::network::RestRequestBuilder{}
          .AddParam("string", "abc")
          .AddParam("milliseconds", std::chrono::milliseconds{123456789})
          .AddParam("int", 123456789)
          .AddParam("int64_t", int64_t{123456789})
          .AddParam("float", float{123456789.123456789})
          .AddParam("double", double{123456789.123456789})
          .AddParam("default_enum_name", DefaultNameEnum::kDefaultEnumName)
          .AddParam("custom_enum_name", CustomNameEnum::kCustomEnumName)
          .AddParam("optional_string", std::optional<std::string>{"abc"})
          .AddParam("optional_milliseconds",
                    std::optional<std::chrono::milliseconds>{123456789})
          .AddParam("optional_int", std::optional<int>{123456789})
          .AddParam("optional_int64_t",
                    std::optional<int64_t>{int64_t{123456789}})
          .AddParam("optional_float",
                    std::optional<float>{float{123456789.123456789}})
          .AddParam("optional_double",
                    std::optional<double>{double{123456789.123456789}})
          .AddParam(
              "optional_default_enum_name",
              std::optional<DefaultNameEnum>{DefaultNameEnum::kDefaultEnumName})
          .AddParam(
              "optional_custom_enum_name",
              std::optional<CustomNameEnum>{CustomNameEnum::kCustomEnumName})
          .AddParam("no_option", std::optional<int>{std::nullopt})
          .Build();
  const auto expected_params = std::map<std::string, std::string>{
      {"string", "abc"},
      {"milliseconds", "123456789"},
      {"int", "123456789"},
      {"int64_t", "123456789"},
      {"float", "123456792"},
      {"double", "123456789.12345679"},
      {"default_enum_name", "kDefaultEnumName"},
      {"custom_enum_name", "CUSTOM_ENUM_NAME"},
      {"optional_string", "abc"},
      {"optional_milliseconds", "123456789"},
      {"optional_int", "123456789"},
      {"optional_int64_t", "123456789"},
      {"optional_float", "123456792"},
      {"optional_double", "123456789.12345679"},
      {"optional_default_enum_name", "kDefaultEnumName"},
      {"optional_custom_enum_name", "CUSTOM_ENUM_NAME"},
  };
  EXPECT_EQ(data.params, expected_params);
}

TEST(RestRequest, SendRequest) {
  const auto [endpoint, data] =
      stonks::network::RestRequestBuilder{}
          .WithBaseUri("https://api.binance.com/api/v3")
          .AppendUri("avgPrice")
          .AddParam("symbol", "BTCUSDT")
          .Build();
  const auto sender =
      std::make_unique<stonks::restsdk::RestRequestSender>(endpoint);
  const auto response = sender->SendRequestAndGetResponse(data);
  const auto &a = response->GetImpl();
  EXPECT_FALSE(a.GetJson().is_null());
}