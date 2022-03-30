#include "rest_request.h"

#include <gtest/gtest.h>

#include <magic_enum.hpp>

TEST(RestRequest, AppendUri) {
  const auto result_uri =
      stonks::rest::RestRequest{"base_uri"}.AppendUri("appended_uri").GetUri();
  const auto exprected_uri = "base_uri/appended_uri";
  EXPECT_EQ(result_uri, exprected_uri);
}

namespace {
enum class DefaultNameEnum { kDefaultEnumName };
enum class CustomNameEnum { kCustomEnumName };
}  // namespace

template <>
constexpr std::string_view magic_enum::customize::enum_name<CustomNameEnum>(
    CustomNameEnum value) noexcept {
  switch (value) {
    case CustomNameEnum::kCustomEnumName:
      return "CUSTOM_ENUM_NAME";
    default:
      break;
  }

  return {};
}

TEST(RestRequest, ParameterTypesToString) {
  const auto result_string =
      stonks::rest::RestRequest{""}
          .AddParameter("string", "abc")
          .AddParameter("int", 123456789)
          .AddParameter("int64_t", int64_t{123456789})
          .AddParameter("float", float{123456789.123456789})
          .AddParameter("double", double{123456789.123456789})
          .AddParameter("default_enum_name", DefaultNameEnum::kDefaultEnumName)
          .AddParameter("custom_enum_name", CustomNameEnum::kCustomEnumName)
          .AddParameter("optional_string", std::optional<std::string>{"abc"})
          .AddParameter("optional_int", std::optional<int>{123456789})
          .AddParameter("optional_int64_t",
                        std::optional<int64_t>{int64_t{123456789}})
          .AddParameter("optional_float",
                        std::optional<float>{float{123456789.123456789}})
          .AddParameter("optional_double",
                        std::optional<double>{double{123456789.123456789}})
          .AddParameter(
              "optional_default_enum_name",
              std::optional<DefaultNameEnum>{DefaultNameEnum::kDefaultEnumName})
          .AddParameter(
              "optional_custom_enum_name",
              std::optional<CustomNameEnum>{CustomNameEnum::kCustomEnumName})
          .AddParameter("no_option", std::optional<int>{std::nullopt})
          .GetParametersAsString();
  const auto expected_string =
      "string=abc&"
      "int=123456789&"
      "int64_t=123456789&"
      "float=123456792&"
      "double=123456789.12345679&"
      "default_enum_name=kDefaultEnumName&"
      "custom_enum_name=CUSTOM_ENUM_NAME&"
      "optional_string=abc&"
      "optional_int=123456789&"
      "optional_int64_t=123456789&"
      "optional_float=123456792&"
      "optional_double=123456789.12345679&"
      "optional_default_enum_name=kDefaultEnumName&"
      "optional_custom_enum_name=CUSTOM_ENUM_NAME";
  EXPECT_EQ(result_string, expected_string);
}