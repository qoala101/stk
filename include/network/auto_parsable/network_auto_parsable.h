#ifndef STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_H_
#define STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_H_

#include <polymorphic_value.h>

#include <any>
#include <gsl/assert>
#include <type_traits>

#include "cpp_optional.h"
#include "cpp_polymorphic_value.h"
#include "network_concepts.h"  // IWYU pragma: keep
#include "network_i_json.h"

namespace stonks::network {
template <Parsable T>
auto ParseFromJson [[nodiscard]] (const IJson &json) -> T;

/**
 * @brief Implicitly converts JSON to any type which can be parsed from it.
 */
class AutoParsable {
 private:
  template <Parsable T>
  auto OperatorTImpl [[nodiscard]] () {
    Expects(json_.has_value());
    auto value = ParseFromJson<std::remove_cvref_t<T>>(**json_);
    json_.reset();
    Ensures(!json_.has_value());
    return value;
  }

 public:
  explicit AutoParsable(cpp::Pv<IJson> json);

  /**
   * @brief Converts JSON to the specified type.
   * @remark Should be called only once.
   */
  template <Parsable T>
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  [[nodiscard]] operator T() {
    return OperatorTImpl<T>();
  }

  /**
   * @brief Converts JSON to the specified type and gives pointer to it.
   * @return Pointer to the internally stored object,
   * or null if JSON has no value.
   * @remark Should be called only once.
   */
  template <Parsable T>
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  [[nodiscard]] operator const T *() {
    Expects(!pointed_.has_value());
    auto value = OperatorTImpl<cpp::Opt<T>>();

    if (!value.has_value()) {
      return nullptr;
    }

    pointed_ = std::move(*value);
    Ensures(pointed_.has_value());
    return std::any_cast<T>(&pointed_);
  }

 private:
  cpp::Opt<cpp::Pv<IJson>> json_;
  std::any pointed_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_H_
