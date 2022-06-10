#ifndef STONKS_ANY_ANY_H_
#define STONKS_ANY_ANY_H_

#include <any>
#include <utility>

namespace stonks::any {
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
  [[nodiscard]] auto Take() && -> T && {
    // NOLINTNEXTLINE(*-const-cast)
    return std::move(const_cast<T &>(const_cast<const Any *>(this)->Get<T>()));
  }

  [[nodiscard]] auto HasValue() const -> bool;

 private:
  std::any value_{};
};
}  // namespace stonks::any

#endif  // STONKS_ANY_ANY_H_
