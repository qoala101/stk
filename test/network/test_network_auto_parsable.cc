

#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

#include <memory>
#include <optional>

#include "cpp_optional.h"
#include "gtest/gtest_pred_impl.h"
#include "network_auto_parsable.h"
#include "network_concepts.h"  // IWYU pragma: keep
#include "network_json_base_conversions.h"
#include "network_json_conversions_facades.h"  // IWYU pragma: keep

namespace {
TEST(AutoParsable, ParsePointer) {
  const auto function = [](const stonks::cpp::Opt<int> &value) -> stonks::cpp::Opt<int> {
    return value;
  };

  auto result = function(
      stonks::network::AutoParsable{stonks::network::ConvertToJson(33)});
  EXPECT_EQ(result, 33);

  result = function(
      stonks::network::AutoParsable{stonks::network::CreateNullJson()});
  EXPECT_FALSE(result.has_value());
}
}  // namespace