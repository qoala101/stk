#ifndef STONKS_JSON_ANY_H_
#define STONKS_JSON_ANY_H_

#include <any>
#include <utility>

namespace stonks::json {
/**
 * @brief Any value with convenient member interface.
 */
class Any {
 public:
  template <typename... Args>
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  Any(Args &&...args) : value_{std::forward<Args>(args)...} {}

  /**
   * @remark Correctness of type is not checked.
   */
  template <typename T>
  [[nodiscard]] auto Get() const -> const T & {
    return std::any_cast<const T &>(value_);
  }

  /**
   * @remark Correctness of type is not checked.
   */
  template <typename T>
  [[nodiscard]] auto Take() -> T && {
    return std::any_cast<T &&>(std::move(value_));
  }

  [[nodiscard]] auto HasValue() const -> bool;

 private:
  std::any value_{};
};
}  // namespace stonks::json

#endif  // STONKS_JSON_ANY_H_
