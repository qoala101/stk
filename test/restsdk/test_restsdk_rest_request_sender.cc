#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <polymorphic_value.h>

#include <cstdint>
#include <magic_enum.hpp>
#include <map>
#include <ostream>
#include <string>
#include <string_view>

#include "cpp_format.h"
#include "cpp_polymorphic_value.h"
#include "gtest/gtest_pred_impl.h"
#include "network_i_json.h"
#include "network_json_basic_conversions.h"
#include "network_json_common_conversions.h"
#include "network_json_conversions_facades.h"
#include "network_rest_request_builder.h"
#include "network_types.h"
#include "restsdk_rest_request_sender.h"
#include "test_restsdk_injector.h"

namespace {
enum class DefaultNameEnum { kDefaultEnumName };
enum class CustomNameEnum { kCustomEnumName };

struct AvgPrice {
  int mins{};
  double price{};

 private:
  friend auto operator==(const AvgPrice &, const AvgPrice &) -> bool = default;
  friend auto operator<<(std::ostream &stream, const AvgPrice &avg_price)
      -> std::ostream & {
    return stream << avg_price.mins << " " << avg_price.price;
  }
};
}  // namespace

template <>
constexpr auto magic_enum::customize::enum_name<CustomNameEnum>(
    CustomNameEnum value) noexcept -> magic_enum::customize::customize_t {
  switch (value) {
    case CustomNameEnum::kCustomEnumName:
      return "CUSTOM_ENUM_NAME";
    default:
      break;
  }

  return invalid_tag;
}

namespace stonks::network {
template <>
auto ParseFromJson(const IJson &json) -> AvgPrice {
  return {.mins = ParseFromJsonChild<int>(json, "mins"),
          .price = std::stod(ParseFromJsonChild<std::string>(json, "price"))};
}

auto ConvertToJson(const AvgPrice &value) -> cpp::Pv<IJson> {
  // clang-format off
  return BuildJsonFrom(
    "mins", value.mins,
    "price", cpp::Format("{}", value.price)
  );
  // clang-format on
}
}  // namespace stonks::network

namespace {
TEST(RestRequestSender, AppendUri) {
  const auto request = stonks::network::RestRequestBuilder{}
                           .WithBaseUri({"base_uri"})
                           .AppendUri({"appended_uri"})
                           .Build();
  EXPECT_EQ(request.endpoint.uri,
            stonks::network::Uri{"base_uri/appended_uri"});
}

TEST(RestRequestSender, ParameterTypesToString) {
  const auto request =
      stonks::network::RestRequestBuilder{}
          .WithBaseUri({})
          .AddParam("string", "abc")
          // .AddParam("milliseconds", std::chrono::milliseconds{123456789})
          .AddParam("int", 123456789)
          .AddParam("int64_t", int64_t{123456789})
          .AddParam("float", float{123456789.123456789})
          .AddParam("double", double{123456789.123456789})
          // .AddParam("default_enum_name", DefaultNameEnum::kDefaultEnumName)
          // .AddParam("custom_enum_name", CustomNameEnum::kCustomEnumName)
          // .AddParam("optional_string", std::optional<std::string>{"abc"})
          // .AddParam("optional_milliseconds",
          //           std::optional<std::chrono::milliseconds>{123456789})
          // .AddParam("optional_int", std::optional<int>{123456789})
          // .AddParam("optional_int64_t",
          //           std::optional<int64_t>{int64_t{123456789}})
          // .AddParam("optional_float",
          //           std::optional<float>{float{123456789.123456789}})
          // .AddParam("optional_double",
          //           std::optional<double>{double{123456789.123456789}})
          // .AddParam(
          //     "optional_default_enum_name",
          //     std::optional<DefaultNameEnum>{DefaultNameEnum::kDefaultEnumName})
          // .AddParam(
          //     "optional_custom_enum_name",
          //     std::optional<CustomNameEnum>{CustomNameEnum::kCustomEnumName})
          // .AddParam("no_option", std::optional<int>{std::nullopt})
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
  // EXPECT_EQ(data.params, expected_params);
}

TEST(RestRequestSender, SendRequest) {
  const auto request = stonks::network::RestRequestBuilder{}
                           .WithBaseUri({"https://api.binance.com/api/v3"})
                           .AppendUri({"avgPrice"})
                           .AddParam("symbol", "BTCUSDT")
                           .Build();
  const auto sender =
      test::restsdk::Injector().create<stonks::restsdk::RestRequestSender>();
  const auto response = sender.SendRequestAndGetResponse(request);
  const auto response_price =
      stonks::network::ParseFromJson<AvgPrice>(**response.result);
  EXPECT_GT(response_price.mins, 0);
  EXPECT_GT(response_price.price, 0);

  const auto response_price_json =
      stonks::network::ConvertToJson(response_price);
  const auto json_price = ParseFromJson<AvgPrice>(*response_price_json);
  EXPECT_EQ(response_price, json_price);
}
}  // namespace