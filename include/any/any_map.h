#ifndef STONKS_ANY_ANY_MAP_H_
#define STONKS_ANY_ANY_MAP_H_

#include <map>
#include <string>
#include <string_view>

#include "any.h"

namespace stonks::any {
/**
 * @brief Map of any values.
 */
class AnyMap {
 public:
  template <typename... Args>
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  AnyMap(Args &&...args) : params_{std::forward<Args>(args)...} {}

  /**
   * @remark Presence of key in the map is not checked.
   * @remark Correctness of type is not checked.
   */
  template <typename T>
  [[nodiscard]] auto Get(std::string_view key) const -> const T & {
    return params_.at(key.data()).Get<T>();
  }

  /**
   * @remark Presence of key in the map is not checked.
   * @remark Correctness of type is not checked.
   */
  template <typename T>
  [[nodiscard]] auto Take(std::string_view key) && -> const T & {
    return std::move(
        // NOLINTNEXTLINE(*-const-cast)
        const_cast<T &>(const_cast<const AnyMap *>(this)->Get<T>(key)));
  }

  void Set(std::string_view key, Any value) {
    params_[key.data()] = std::move(value);
  }

 private:
  std::map<std::string, Any> params_{};
};
}  // namespace stonks::any

#endif  // STONKS_ANY_ANY_MAP_H_
