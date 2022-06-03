#ifndef STONKS_JSON_ANY_MAP_H_
#define STONKS_JSON_ANY_MAP_H_

#include <map>
#include <string>
#include <string_view>

#include "any.h"

namespace stonks::json {
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
  [[nodiscard]] auto Get(std::string_view key) -> const T & {
    return params_.at(key.data()).Get<T>();
  }

  /**
   * @remark Presence of key in the map is not checked.
   * @remark Correctness of type is not checked.
   */
  template <typename T>
  [[nodiscard]] auto Take(std::string_view key) -> T && {
    return params_.at(key.data()).Take<T>();
  }

  void Set(std::string_view key, json::Any value) {
    params_[key.data()] = std::move(value);
  }

 private:
  std::map<std::string, json::Any> params_{};
};
}  // namespace stonks::json

#endif  // STONKS_JSON_ANY_MAP_H_
