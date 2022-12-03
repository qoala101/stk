#ifndef STONKS_SQLDB_SQLDB_VALUE_H_
#define STONKS_SQLDB_SQLDB_VALUE_H_

#include <cstdint>
#include <string>
#include <utility>
#include <variant>

#include "cpp_concepts.h"
#include "cpp_this.h"  // IWYU pragma: keep
#include "cpp_variant_struct.h"
#include "sqldb_concepts.h"  // IWYU pragma: keep
#include "sqldb_types.h"

namespace stonks::sqldb {
namespace detail {
template <SupportedDataType T>
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
  template <SupportedDataType T>
  auto Get [[nodiscard]] () const ->
      typename detail::GetResultTraits<T>::ConstType;

  template <SupportedDataType T>
  auto Get [[nodiscard]] () -> typename detail::GetResultTraits<T>::Type;

  /**
   * @remark Should not be called on NULL value.
   */
  auto GetType [[nodiscard]] () const -> DataTypeVariant;

  /**
   * @brief Tells whether there is a value of any type.
   */
  auto IsNull [[nodiscard]] () const -> bool;

 private:
  template <SupportedDataType T>
  static auto GetImpl [[nodiscard]] (cpp::This<Value> auto &t) -> auto &;
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_VALUE_H_
