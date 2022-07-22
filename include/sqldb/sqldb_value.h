#ifndef STONKS_SQLDB_SQLDB_VALUE_H_
#define STONKS_SQLDB_SQLDB_VALUE_H_

#include <cstdint>
#include <string>
#include <string_view>
#include <variant>

#include "sqldb_enums.h"

namespace stonks::sqldb {
/**
 * @brief Value which can be stored or retrieved from DB.
 */
class Value {
 public:
  /**
   * @brief Parses value of specified type from string.
   */
  explicit Value(std::string_view value, DataType data_type);

  /**
   * @brief Constructs NULL value.
   */
  Value() = default;

  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  Value(int value);
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  Value(int64_t value);
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  Value(double value);
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  Value(std::string_view value);

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

  [[nodiscard]] static auto GetStringImpl(auto &t) -> auto &;

  std::variant<std::monostate, int, int64_t, double, std::string> value_{};
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_VALUE_H_
