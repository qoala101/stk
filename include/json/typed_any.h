#ifndef STONKS_JSON_TYPED_ANY_H_
#define STONKS_JSON_TYPED_ANY_H_

#include <any>
#include <type_traits>

#include "type_variant.h"

/**
 * @brief Stores any value, knows its type and how to convert it.
 */
namespace stonks::json {
class TypedAny : public TypeVariant {
 public:
  TypedAny() = default;

  template <typename T>
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  TypedAny(T value)
      : TypeVariant{Type<std::decay_t<T>>{}}, value_{std::move(value)} {}

  [[nodiscard]] auto GetAny() const -> const std::any&;

 private:
  std::any value_{};
};
}  // namespace stonks::json

#endif  // STONKS_JSON_TYPED_ANY_H_
