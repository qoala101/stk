#ifndef STONKS_DB_DB_VALUE_H_
#define STONKS_DB_DB_VALUE_H_

#include <cstdint>
#include <string>
#include <string_view>
#include <variant>

#include "db_enums.h"

namespace stonks::db {
/**
 * @brief Value which can be stored or retrieved from DB.
 */
class Value {
 public:
  /**
   * @brief Parses value of specified type from string.
   */
  explicit Value(std::string_view value, DataType data_type);

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
  [[nodiscard]] auto GetString() const & -> const std::string &;
  [[nodiscard]] auto GetString() && -> std::string &&;

  [[nodiscard]] auto GetType() const -> DataType;

  /**
   * @brief Converts value of any type to string.
   */
  // TODO(vh): needed only for Insert
  [[nodiscard]] auto ToString() const -> std::string;

 private:
  std::variant<int, int64_t, double, std::string> value_{};
};
}  // namespace stonks::db

#endif  // STONKS_DB_DB_VALUE_H_
