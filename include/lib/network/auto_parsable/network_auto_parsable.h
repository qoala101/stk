/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_AUTO_PARSABLE_H_
#define VH_NETWORK_AUTO_PARSABLE_H_

#include <polymorphic_value.h>

#include <any>
#include <gsl/assert>
#include <type_traits>

#include "cpp_optional.h"
#include "cpp_polymorphic_value.h"
#include "network_concepts.h"  // IWYU pragma: keep
#include "network_i_json.h"

namespace vh::network {
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

  // TODO(vh): This is a hack which allows AutoParsable to be converted
  // to const cpp::Opt<T> &. Conversion to cpp::Opt<T> value doesn't work
  // because of ambiguity. This has to be fixed.
  /**
   * @copydoc operator T
   */
  template <Parsable T>
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  [[nodiscard]] operator const cpp::Opt<T> &() {
    Expects(!optional_.has_value());
    optional_ = OperatorTImpl<cpp::Opt<T>>();
    Ensures(optional_.has_value());
    return std::any_cast<const cpp::Opt<T> &>(optional_);
  }

 private:
  cpp::Opt<cpp::Pv<IJson>> json_;
  std::any optional_{};
};
}  // namespace vh::network

#endif  // VH_NETWORK_AUTO_PARSABLE_H_
