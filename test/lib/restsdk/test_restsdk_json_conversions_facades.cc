#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

#include "cpp_polymorphic_value.h"
#include "gtest/gtest_pred_impl.h"
#include "network_i_json.h"
#include "network_json_conversions_facades.h"

namespace {
struct BaseTypes {
  bool a{};
  int b{};
  int64_t c{};
  double d{};
  std::string e{};

 private:
  friend auto operator== [[nodiscard]] (const BaseTypes &, const BaseTypes &)
  -> bool = default;
};

TEST(JsonConversionsFacades, StringIndex) {
  const auto original = BaseTypes{true, std::numeric_limits<int>::max(),
                                  std::numeric_limits<int64_t>::max(),
                                  std::numeric_limits<double>::max(), "text"};

  // clang-format off
  auto json = vh::network::BuildJsonFrom(
    "a", original.a,
    "b", original.b,
    "c", original.c,
    "d", original.d,
    "e", original.e
  );
  // clang-format on

  const auto parsed_const = vh::network::MakeFromJson<BaseTypes>(
      const_cast<const vh::network::IJson &>(*json), "a", "b", "c", "d", "e");
  const auto parsed_non_const =
      vh::network::MakeFromJson<BaseTypes>(*json, "a", "b", "c", "d", "e");

  EXPECT_EQ(original, parsed_const);
  EXPECT_EQ(original, parsed_non_const);
}

TEST(JsonConversionsFacades, IntIndex) {
  const auto original = std::vector<std::string>{"a", "b", "c", "d", "e"};

  // clang-format off
  auto json = vh::network::BuildJsonFrom(
    0, original[0],
    1, original[1],
    2, original[2],
    3, original[3],
    4, original[4]
  );
  // clang-format on

  const auto parsed_const = vh::network::MakeFromJson<std::vector<std::string>>(
      const_cast<const vh::network::IJson &>(*json), 0, 1, 2, 3, 4);
  const auto parsed_non_const =
      vh::network::MakeFromJson<std::vector<std::string>>(*json, 0, 1, 2, 3, 4);

  EXPECT_EQ(original, parsed_const);
  EXPECT_EQ(original, parsed_non_const);
}
}  // namespace