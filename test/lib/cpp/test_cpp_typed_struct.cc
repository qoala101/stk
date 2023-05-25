/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

#include <string>
#include <utility>

#include "cpp_typed_struct.h"
#include "gtest/gtest_pred_impl.h"

namespace {
struct TypedString : public vh::cpp::TypedStruct<std::string> {};

TEST(TypedStruct, RvalueConversion) {
  auto typed_s = TypedString{"value"};
  EXPECT_FALSE(std::string{typed_s}.empty());

  const auto string = std::string{std::move(typed_s)};
  EXPECT_EQ(string, "value");
  EXPECT_TRUE(std::string{typed_s}.empty());
}
}  // namespace