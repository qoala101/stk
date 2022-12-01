#ifndef STONKS_CPP_CPP_TYPED_STRUCT_H_
#define STONKS_CPP_CPP_TYPED_STRUCT_H_

#include <compare>
#include <ostream>
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
  auto operator* [[nodiscard]] () const -> auto & {
    return OperatorAsteriskImpl(*this);
  }

  auto operator* [[nodiscard]] () -> auto & {
    return OperatorAsteriskImpl(*this);
  }

  /**
   * @brief Gives the value.
   */
  auto operator->[[nodiscard]] () const { return OperatorArrowImpl(*this); }

  auto operator->[[nodiscard]] () { return OperatorArrowImpl(*this); }

  ValueType value{};

 private:
  friend auto operator== [[nodiscard]] (const TypedStruct &,
                                        const TypedStruct &) -> bool = default;
  friend auto operator<=>
      [[nodiscard]] (const TypedStruct &, const TypedStruct &)
      -> std::partial_ordering = default;

  friend auto operator<<(std::ostream &stream,
                         const TypedStruct<ValueType> &value) -> auto & {
    return stream << *value;
  }

  template <This<TypedStruct> This>
  static auto OperatorValueTypeImpl [[nodiscard]] (This &&t) {
    return MoveIfRvalue<decltype(std::forward<This>(t))>(*t);
  }

  static auto OperatorAsteriskImpl [[nodiscard]] (This<TypedStruct> auto &t)
  -> auto & {
    return t.value;
  }

  static auto OperatorArrowImpl [[nodiscard]] (This<TypedStruct> auto &t) {
    return AssumeNn(&*t);
  }
};
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_TYPED_STRUCT_H_
