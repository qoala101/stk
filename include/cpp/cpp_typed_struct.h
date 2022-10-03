#ifndef STONKS_CPP_CPP_TYPED_STRUCT_H_
#define STONKS_CPP_CPP_TYPED_STRUCT_H_

#include <compare>
#include <utility>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_move_if.h"

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

  ValueType value{};

 private:
  [[nodiscard]] friend auto operator==(const TypedStruct &, const TypedStruct &)
      -> bool = default;
  [[nodiscard]] friend auto operator<=>(const TypedStruct &,
                                        const TypedStruct &)
      -> std::partial_ordering = default;

  template <DecaysTo<TypedStruct> This>
  [[nodiscard]] static auto OperatorValueTypeImpl(This &&t) {
    return MoveIfRvalue<decltype(std::forward<This>(t))>(t.value);
  }
};
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_TYPED_STRUCT_H_
