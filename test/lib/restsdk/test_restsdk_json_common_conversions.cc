/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

#include <exception>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "cpp_optional.h"
#include "cpp_polymorphic_value.h"
#include "gtest/gtest_pred_impl.h"
#include "network_json_base_conversions.h"
#include "network_json_common_conversions.h"
#include "network_json_conversions_facades.h"

namespace {
TEST(JsonSpecializedConversions, ConvertAndParseOptional) {
  const auto text = std::string{"HELLO"};
  const auto text_json = vh::network::ConvertToJson(text);
  const auto parsed_text =
      vh::network::ParseFromJson<vh::cpp::Opt<std::string>>(*text_json);

  ASSERT_TRUE(parsed_text.has_value());
  EXPECT_EQ(*parsed_text, text);

  const auto *text_ptr = &text;
  const auto text_ptr_json = vh::network::ConvertToJson(text_ptr);
  const auto parsed_text_ptr =
      vh::network::ParseFromJson<vh::cpp::Opt<std::string>>(*text_ptr_json);

  ASSERT_TRUE(parsed_text_ptr.has_value());
  EXPECT_EQ(*parsed_text_ptr, *text_ptr);

  const auto *text_null = decltype(&text_ptr){};
  const auto text_null_json = vh::network::ConvertToJson(text_null);
  const auto parsed_text_null =
      vh::network::ParseFromJson<vh::cpp::Opt<std::string>>(*text_null_json);

  EXPECT_FALSE(parsed_text_null.has_value());
}

TEST(JsonSpecializedConversions, ConvertAndParseVector) {
  const auto items = std::vector<std::string>{"a", "b", "c"};
  const auto items_json = vh::network::ConvertToJson(items);
  const auto parsed_items =
      vh::network::ParseFromJson<std::vector<std::string>>(*items_json);

  EXPECT_EQ(parsed_items, items);
}

TEST(JsonSpecializedConversions, ConvertAndParseOptionalVector) {
  const auto items = std::vector<std::string>{"a", "b", "c"};

  const auto *items_ptr = &items;
  const auto items_ptr_json = vh::network::ConvertToJson(items_ptr);
  const auto parsed_items_ptr =
      vh::network::ParseFromJson<vh::cpp::Opt<std::vector<std::string>>>(
          *items_ptr_json);

  ASSERT_TRUE(parsed_items_ptr.has_value());
  EXPECT_EQ(*parsed_items_ptr, *items_ptr);

  const auto *items_null = decltype(&items_ptr){};
  const auto items_null_json = vh::network::ConvertToJson(items_null);
  const auto parsed_items_null =
      vh::network::ParseFromJson<vh::cpp::Opt<std::vector<std::string>>>(
          *items_null_json);

  EXPECT_FALSE(parsed_items_null.has_value());
}

TEST(JsonSpecializedConversions, ConvertAndParseVariant) {
  using Variant = std::variant<std::string, int, bool>;

  const auto values = std::vector<Variant>{{"Text"}, {32}, {true}};
  const auto jsons = vh::network::ConvertToJson(values);
  const auto parsed_values =
      vh::network::ParseFromJson<std::vector<Variant>>(*jsons);

  EXPECT_EQ(parsed_values, values);
}

TEST(JsonSpecializedConversions, ConvertAndParseVariantMonostate) {
  using Variant = std::variant<std::string, int, bool>;
  using VariantMonostate = std::variant<std::monostate, std::string, int, bool>;

  const auto null_json = vh::network::CreateNullJson();

  EXPECT_ANY_THROW(const auto parsed_value =
                       vh::network::ParseFromJson<Variant>(*null_json));

  const auto parsed_value =
      vh::network::ParseFromJson<VariantMonostate>(*null_json);
  EXPECT_EQ(parsed_value.index(), 0);
}
}  // namespace