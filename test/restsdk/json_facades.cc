#include <gtest/gtest.h>

#include <cstdint>
#include <limits>

#include "network_json_facades_aliases_p_c.h"

namespace {
struct BaseTypes {
  bool a{};
  int b{};
  int64_t c{};
  double d{};
  std::string e{};

 private:
  [[nodiscard]] friend auto operator==(const BaseTypes &, const BaseTypes &)
      -> bool = default;
};

TEST(RestRequestReceiver, SendRequest) {
  const auto original = BaseTypes{true, std::numeric_limits<int>::max(),
                                  std::numeric_limits<int64_t>::max(),
                                  std::numeric_limits<double>::max(), "text"};

  // clang-format off
  const auto json = C(
    "a", original.a,
    "b", original.b,
    "c", original.c,
    "d", original.d,
    "e", original.e
  );
  // clang-format on

  const auto parsed = P<BaseTypes>(*json, "a", "b", "c", "d", "e");

  EXPECT_EQ(original, parsed);
}
}  // namespace