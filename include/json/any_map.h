#ifndef STONKS_JSON_ANY_MAP_H_
#define STONKS_JSON_ANY_MAP_H_

#include <any>
#include <map>
#include <string>
#include <string_view>

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
   */
  template <typename T>
  [[nodiscard]] auto Get(std::string_view key) -> T {
    return std::any_cast<T>(std::move(params_.at(key.data())));
  }

 private:
  std::map<std::string, std::any> params_{};
};
}  // namespace stonks::json

#endif  // STONKS_JSON_ANY_MAP_H_
