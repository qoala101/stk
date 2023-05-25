/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

#include <memory>
#include <utility>

#include "cpp_move_if.h"
#include "gtest/gtest_pred_impl.h"

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

template <typename T>
void NotConstSetSImpl(T &&t) {
  auto s = S{};
  t.s_ = vh::cpp::MoveIfNotConst<T>(s);
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

template <typename T>
void RvalueSetSImpl(T &&t) {
  auto s = S{};
  t.s_ = vh::cpp::MoveIfRvalue<decltype(std::forward<T>(t))>(s);
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