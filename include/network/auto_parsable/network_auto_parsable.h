#ifndef STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_H_
#define STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_H_

#include <type_traits>

#include "cpp_polymorphic_value.h"
#include "network_concepts.h"  // IWYU pragma: keep
#include "network_i_json.h"

namespace stonks::network {
/**
 * @brief Implicitly converts JSON to any type which can be parsed from it.
 */
class AutoParsable {
 public:
  explicit AutoParsable(cpp::Pv<IJson> json);

  /**
   * @brief Converts JSON to the specified type.
   */
  template <Parsable T>
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  operator T() {
    return ParseFromJson<std::decay_t<T>>(*json_);
  }

 private:
  cpp::Pv<IJson> json_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_H_
