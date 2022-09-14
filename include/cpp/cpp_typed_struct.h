#ifndef STONKS_CPP_CPP_TYPED_STRUCT_H_
#define STONKS_CPP_CPP_TYPED_STRUCT_H_

#include <compare>
#include <ostream>
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

  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  operator T() const & { return OperatorTImpl(*this); }
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  operator T() & { return OperatorTImpl(*this); }
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  operator T() && { return OperatorTImpl(std::move(*this)); }

  T value{};

 private:
  [[nodiscard]] friend auto operator==(const TypedStruct &, const TypedStruct &)
      -> bool = default;
  [[nodiscard]] friend auto operator<=>(const TypedStruct &,
                                        const TypedStruct &)
      -> std::partial_ordering = default;
  [[nodiscard]] friend auto operator<<(std::ostream &stream,
                                       const TypedStruct &typed_struct)
      -> std::ostream & {
    return stream << typed_struct.value;
  }

  template <DecaysTo<TypedStruct> This>
  [[nodiscard]] static auto OperatorTImpl(This &&t) -> T {
    return MoveIfRvalue<decltype(std::forward<This>(t))>(t.value);
  }
};
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_TYPED_STRUCT_H_
