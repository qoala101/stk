#ifndef STONKS_SQLDB_SQLDB_VALUE_H_
#define STONKS_SQLDB_SQLDB_VALUE_H_

#include <cstdint>
#include <string>
#include <string_view>
#include <variant>

#include "cpp_copy_const.h"
#include "sqldb_enums.h"

namespace stonks::sqldb {
/**
 * @brief Value which can be stored or retrieved from DB.
 */
class Value {
 public:
  /**
   * @brief Constructs null value.
   */
  Value() = default;

  /**
   * @brief Parses value of specified type from string.
   */
  Value(std::string_view value, DataType data_type);

  explicit Value(bool value);
  explicit Value(int value);
  explicit Value(int64_t value);
  explicit Value(double value);
  explicit Value(const char *value);
  explicit Value(std::string value);

  [[nodiscard]] auto GetBool() const -> bool;

  [[nodiscard]] auto GetInt() const -> int;

  [[nodiscard]] auto GetInt64() const -> int64_t;

  [[nodiscard]] auto GetDouble() const -> double;

  [[nodiscard]] auto GetString() const -> const std::string &;
  [[nodiscard]] auto GetString() -> std::string &;

  /**
   * @remark Should not be called on NULL value.
   */
  [[nodiscard]] auto GetType() const -> DataType;

  [[nodiscard]] auto IsNull() const -> bool;

 private:
  [[nodiscard]] friend auto operator==(const Value &, const Value &)
      -> bool = default;

  template <cpp::DecaysTo<Value> This>
  [[nodiscard]] static auto GetStringImpl(This &t)
      -> cpp::CopyConst<This, std::string &>;

  std::variant<std::monostate, bool, int, int64_t, double, std::string>
      value_{};
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_VALUE_H_
