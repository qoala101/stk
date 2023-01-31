#ifndef STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_WRONG_TYPE_EXCEPTION_H_
#define STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_WRONG_TYPE_EXCEPTION_H_

#include <string>

#include "cpp_polymorphic_value.h"
#include "cpp_this.h"  // IWYU pragma: keep
#include "network_exception.h"
#include "network_i_json.h"

namespace vh::network {
/**
 * @brief Exception produced by the attempt to parse a JSON.
 */
class WrongTypeException : public Exception {
 public:
  /**
   * @param message Description of parsing error.
   * @param json JSON which failed to be parsed
   */
  WrongTypeException(std::string message, cpp::Pv<IJson> json);

  /**
   * @brief Gives JSON which failed to be parsed.
   */
  auto GetJson [[nodiscard]] () const -> const cpp::Pv<IJson> &;
  auto GetJson [[nodiscard]] () -> cpp::Pv<IJson> &;

 private:
  static auto GetJsonImpl [[nodiscard]] (cpp::This<WrongTypeException> auto &t)
  -> auto &;

  cpp::Pv<IJson> json_{};
};
}  // namespace vh::network

#endif  // STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_WRONG_TYPE_EXCEPTION_H_
