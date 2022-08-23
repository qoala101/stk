#ifndef STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_WRONG_TYPE_EXCEPTION_H_
#define STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_WRONG_TYPE_EXCEPTION_H_

#include <string>

#include "cpp_message_exception.h"
#include "cpp_polymorphic_value.h"
#include "network_i_json.h"

namespace stonks::network {
/**
 * @brief Exception produced by the attempt to a JSON.
 */
class WrongTypeException : public cpp::MessageException {
 public:
  /**
   * @param message Description of parsing error.
   * @param json JSON which failed to be parsed
   */
  explicit WrongTypeException(std::string message, cpp::Pv<IJson> json);

  /**
   * @brief Gives JSON which failed to be parsed.
   */
  [[nodiscard]] auto GetJson() const -> const cpp::Pv<IJson> &;

  /**
   * @copydoc GetJson
   */
  [[nodiscard]] auto GetJson() -> cpp::Pv<IJson> &;

 private:
  [[nodiscard]] static auto GetJsonImpl(auto &t) -> auto &;

  cpp::Pv<IJson> json_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_WRONG_TYPE_EXCEPTION_H_
