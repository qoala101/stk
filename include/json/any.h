#ifndef STONKS_JSON_ANY_H_
#define STONKS_JSON_ANY_H_

#include <any>

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
  [[nodiscard]] auto Get() -> T {
    return std::any_cast<T>(std::move(value_));
  }

 private:
  std::any value_{};
};
}  // namespace stonks::json

#endif  // STONKS_JSON_ANY_H_
