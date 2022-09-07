#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <polymorphic_value.h>

#include <optional>
#include <string>
#include <vector>

#include "cpp_optional.h"
#include "gtest/gtest_pred_impl.h"
#include "network_json_common_conversions.h"
#include "network_json_common_conversions.h"

namespace {
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

TEST(JsonSpecializedConversions, ConvertAndParseVector) {
  const auto items = std::vector<std::string>{"a", "b", "c"};
  const auto items_json = stonks::network::ConvertToJson(items);
  const auto parsed_items =
      stonks::network::ParseFromJson<std::vector<std::string>>(*items_json);

  EXPECT_EQ(parsed_items, items);
}

TEST(JsonSpecializedConversions, ConvertAndParseOptionalVector) {
  const auto items = std::vector<std::string>{"a", "b", "c"};

  const auto *items_ptr = &items;
  const auto items_ptr_json = stonks::network::ConvertToJson(items_ptr);
  const auto parsed_items_ptr = stonks::network::ParseFromJson<
      stonks::cpp::Opt<std::vector<std::string>>>(*items_ptr_json);

  ASSERT_TRUE(parsed_items_ptr.has_value());
  EXPECT_EQ(*parsed_items_ptr, *items_ptr);

  const auto *items_null = decltype(&items_ptr){};
  const auto items_null_json = stonks::network::ConvertToJson(items_null);
  const auto parsed_items_null = stonks::network::ParseFromJson<
      stonks::cpp::Opt<std::vector<std::string>>>(*items_null_json);

  EXPECT_FALSE(parsed_items_null.has_value());
}
}  // namespace