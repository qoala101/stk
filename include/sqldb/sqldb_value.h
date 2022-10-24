#ifndef STONKS_SQLDB_SQLDB_VALUE_H_
#define STONKS_SQLDB_SQLDB_VALUE_H_

#include <cstdint>
#include <string>
#include <utility>
#include <variant>

#include "cpp_this.h"  // IWYU pragma: keep
#include "cpp_typed_struct.h"
#include "cpp_variant_struct.h"
#include "sqldb_types.h"

namespace stonks::sqldb {
namespace detail {
template <typename T>
struct GetResultTraits {
  using Type = T;
  using ConstType = T;
};

template <>
struct GetResultTraits<std::string> {
  using Type = std::string &;
  using ConstType = const std::string &;
};
}  // namespace detail

/**
 * @brief Value which can be stored or retrieved from DB.
 */
class Value : public cpp::VariantStruct<std::monostate, bool, int, int64_t,
                                        double, std::string> {
 public:
  Value() = default;

  template <typename... Args>
    requires cpp::ConstructibleFrom<ValueType, Args...>
  explicit Value(Args &&...args) : BaseType{std::forward<Args>(args)...} {}

  /**
   * @brief Gives the value of specified type.
   */
  template <typename T>
  [[nodiscard]] auto Get() const ->
      typename detail::GetResultTraits<T>::ConstType;

  template <typename T>
  [[nodiscard]] auto Get() -> typename detail::GetResultTraits<T>::Type;

  /**
   * @remark Should not be called on NULL value.
   */
  [[nodiscard]] auto GetType() const -> DataTypeVariant;

  /**
   * @brief Tells whether there is a value of any type.
   */
  [[nodiscard]] auto IsNull() const -> bool;

 private:
  template <typename T>
  [[nodiscard]] static auto GetImpl(cpp::This<Value> auto &t) -> auto &;
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_VALUE_H_
