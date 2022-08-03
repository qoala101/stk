#ifndef STONKS_NETWORK_NETWORK_AUTO_PARSABLE_H_
#define STONKS_NETWORK_NETWORK_AUTO_PARSABLE_H_

#include <polymorphic_value.h>

#include <type_traits>

#include "network_concepts.h"  // IWYU pragma: keep
#include "network_i_json.h"
#include "network_json_basic_conversions.h"

namespace stonks::network {
/**
 * @brief Implicitly converts JSON to any type which can be parsed from it.
 */
class AutoParsable {
 public:
  explicit AutoParsable(isocpp_p0201::polymorphic_value<IJson> json);

  AutoParsable(const AutoParsable &) = delete;
  AutoParsable(AutoParsable &&) noexcept = default;

  auto operator=(const AutoParsable &) -> AutoParsable & = delete;
  auto operator=(AutoParsable &&) noexcept -> AutoParsable & = default;

  ~AutoParsable() = default;

  /**
   * @brief Converts JSON to the specified type.
   */
  template <Parsable T>
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  operator T() {
    return ParseFromJson<std::decay_t<T>>(*json_);
  }

 private:
  isocpp_p0201::polymorphic_value<IJson> json_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_AUTO_PARSABLE_H_
