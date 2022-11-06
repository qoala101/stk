#ifndef STONKS_CPP_CPP_TYPED_STRUCT_H_
#define STONKS_CPP_CPP_TYPED_STRUCT_H_

#include <compare>
#include <utility>

#include "cpp_move_if.h"
#include "cpp_not_null.h"
#include "cpp_this.h"  // IWYU pragma: keep

namespace stonks::cpp {
/**
 * @brief Type-safe replacement for type aliases.
 * Can only be constructed explicitly and implicitly converts
 * to underlying type.
 */
template <typename T>
struct TypedStruct {
  using ValueType = T;
  using BaseType = TypedStruct<ValueType>;

  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  [[nodiscard]] operator ValueType() const & {
    return OperatorValueTypeImpl(*this);
  }

  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  [[nodiscard]] operator ValueType() & { return OperatorValueTypeImpl(*this); }

  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  [[nodiscard]] operator ValueType() && {
    return OperatorValueTypeImpl(std::move(*this));
  }

  /**
   * @brief Gives the value.
   */
  [[nodiscard]] auto operator*() const -> auto & {
    return OperatorAsteriskImpl(*this);
  }

  [[nodiscard]] auto operator*() -> auto & {
    return OperatorAsteriskImpl(*this);
  }

  /**
   * @brief Gives the value.
   */
  [[nodiscard]] auto operator->() const { return OperatorArrowImpl(*this); }

  [[nodiscard]] auto operator->() { return OperatorArrowImpl(*this); }

  ValueType value{};

 private:
  [[nodiscard]] friend auto operator==(const TypedStruct &, const TypedStruct &)
      -> bool = default;
  [[nodiscard]] friend auto operator<=>(const TypedStruct &,
                                        const TypedStruct &)
      -> std::partial_ordering = default;

  template <This<TypedStruct> This>
  [[nodiscard]] static auto OperatorValueTypeImpl(This &&t) {
    return MoveIfRvalue<decltype(std::forward<This>(t))>(*t);
  }

  [[nodiscard]] static auto OperatorAsteriskImpl(This<TypedStruct> auto &t)
      -> auto & {
    return t.value;
  }

  [[nodiscard]] static auto OperatorArrowImpl(This<TypedStruct> auto &t) {
    return AssumeNn(&*t);
  }
};
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_TYPED_STRUCT_H_
