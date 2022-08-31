#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>

#include <memory>
#include <type_traits>
#include <utility>

#include "cpp_move_if.h"

namespace {
struct S {
  explicit S() = default;
  S(const S &) = default;
  S(S && /*unused*/) noexcept : move_constructed_{true} {}
  auto operator=(const S &) -> S & = default;
  auto operator=(S && /*unused*/) noexcept -> S & {
    move_constructed_ = true;
    return *this;
  }

  bool move_constructed_{};
};

void NotConstSetSImpl(auto &&t) {
  auto s = S{};
  t.s_ = stonks::cpp::MoveIfNotConst<decltype(t)>(s);
}

TEST(MoveIf, IfNotConst) {
  struct A {
    void SetS() const { NotConstSetSImpl(*this); }
    void SetS() { NotConstSetSImpl(*this); }

    mutable S s_{};
  };

  const auto a = A{};
  EXPECT_FALSE(a.s_.move_constructed_);

  a.SetS();
  EXPECT_FALSE(a.s_.move_constructed_);

  const_cast<A &>(a).SetS();
  EXPECT_TRUE(a.s_.move_constructed_);
}

void RvalueSetSImpl(auto &&t) {
  auto s = S{};
  t.s_ = stonks::cpp::MoveIfRvalue<decltype(t)>(s);
}

TEST(MoveIf, IfRvalue) {
  struct A {
    void SetS() & { RvalueSetSImpl(*this); }
    void SetS() && { RvalueSetSImpl(std::move(*this)); }

    S s_{};
  };

  auto a = A{};
  EXPECT_FALSE(a.s_.move_constructed_);

  a.SetS();
  EXPECT_FALSE(a.s_.move_constructed_);

  std::move(a).SetS();
  EXPECT_TRUE(a.s_.move_constructed_);
}
}  // namespace