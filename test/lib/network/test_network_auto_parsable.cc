/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

#include <memory>

#include "cpp_optional.h"
#include "gtest/gtest_pred_impl.h"
#include "network_auto_parsable.h"
#include "network_concepts.h"  // IWYU pragma: keep
#include "network_json_base_conversions.h"
#include "network_json_conversions_facades.h"  // IWYU pragma: keep

namespace {
TEST(AutoParsable, ParsePointer) {
  const auto function =
      [](const vh::cpp::Opt<int> &value) -> vh::cpp::Opt<int> { return value; };

  auto result =
      function(vh::network::AutoParsable{vh::network::ConvertToJson(33)});
  EXPECT_EQ(result, 33);

  result = function(vh::network::AutoParsable{vh::network::CreateNullJson()});
  EXPECT_FALSE(result.has_value());
}
}  // namespace