#ifndef STONKS_SQLDB_SQLDB_VALUE_H_
#define STONKS_SQLDB_SQLDB_VALUE_H_

#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <variant>

#include "cpp_this.h"  // IWYU pragma: keep
#include "cpp_typed_struct.h"
#include "cpp_variant_struct.h"
#include "sqldb_enums.h"

namespace stonks::sqldb {
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
   * @brief Parses value of specified type from string.
   */
  Value(std::string_view value, DataType data_type);

  /**
   * @remark Should not be called on NULL value.
   */
  [[nodiscard]] auto GetBool() const -> bool;

  /**
   * @remark Should not be called on NULL value.
   */
  [[nodiscard]] auto GetInt() const -> int;

  /**
   * @remark Should not be called on NULL value.
   */
  [[nodiscard]] auto GetInt64() const -> int64_t;

  /**
   * @remark Should not be called on NULL value.
   */
  [[nodiscard]] auto GetDouble() const -> double;

  /**
   * @remark Should not be called on NULL value.
   */
  [[nodiscard]] auto GetString() const -> const std::string &;
  [[nodiscard]] auto GetString() -> std::string &;

  /**
   * @remark Should not be called on NULL value.
   */
  [[nodiscard]] auto GetType() const -> DataType;

  /**
   * @brief Tells whether there is a value of any type.
   */
  [[nodiscard]] auto IsNull() const -> bool;

 private:
  [[nodiscard]] static auto GetStringImpl(cpp::This<Value> auto &t) -> auto &;
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_VALUE_H_
