#include <gtest/gtest.h>

#include "network_json_basic_conversions.h"
#include "network_json_template_conversions.h"

TEST(JsonSpecializedConversions, ConvertAndParseOptional) {
  const auto text = std::string{"HELLO"};
  const auto text_json = stonks::network::ConvertToJson(text);
  const auto parsed_text =
      stonks::network::ParseFromJson<stonks::cpp::Opt<std::string>>(*text_json);

  ASSERT_TRUE(parsed_text.has_value());
  EXPECT_EQ(*parsed_text, text);

  const auto *text_ptr = &text;
  const auto text_ptr_json = stonks::network::ConvertToJson(text_ptr);
  const auto parsed_text_ptr =
      stonks::network::ParseFromJson<stonks::cpp::Opt<std::string>>(
          *text_ptr_json);

  ASSERT_TRUE(parsed_text_ptr.has_value());
  EXPECT_EQ(*parsed_text_ptr, *text_ptr);

  const auto *text_null = decltype(&text_ptr){};
  const auto text_null_json = stonks::network::ConvertToJson(text_null);
  const auto parsed_text_null =
      stonks::network::ParseFromJson<stonks::cpp::Opt<std::string>>(
          *text_null_json);

  EXPECT_FALSE(parsed_text_null.has_value());
}

TEST(JsonSpecializedConversions, ConvertAndParseVector) {}

TEST(JsonSpecializedConversions, ConvertAndParseOptionalVector) {}