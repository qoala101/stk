#ifndef STONKS_RESTSDK_RESTSDK_JSON_NATIVE_HANDLE_H_
#define STONKS_RESTSDK_RESTSDK_JSON_NATIVE_HANDLE_H_

#include "cpp_not_null.h"
#include "cpp_this.h"  // IWYU pragma: keep
#include "network_i_json.h"

namespace web::json {
class value;
}  // namespace web::json

namespace stonks::network {
/**
 * @brief JSON implementation details.
 */
class IJson::NativeHandle {
 public:
  NativeHandle();
  explicit NativeHandle(const web::json::value &json);
  explicit NativeHandle(web::json::value &&json);

  NativeHandle(const NativeHandle &other);
  NativeHandle(NativeHandle &&) noexcept;

  auto operator=(const NativeHandle &other) -> NativeHandle &;
  auto operator=(NativeHandle &&) noexcept -> NativeHandle &;

  ~NativeHandle();

  /**
   * @brief Gives REST SDK JSON handle.
   */
  auto operator* [[nodiscard]] () const -> const web::json::value &;
  auto operator* [[nodiscard]] () -> web::json::value &;

  /**
   * @brief Gives REST SDK JSON handle.
   */
  auto operator->[[nodiscard]] () const -> cpp::Nn<const web::json::value *>;
  auto operator->[[nodiscard]] () -> cpp::Nn<web::json::value *>;

 private:
  static auto OperatorAsteriskImpl
      [[nodiscard]] (cpp::This<NativeHandle> auto &t) -> auto &;

  static auto OperatorArrowImpl [[nodiscard]] (cpp::This<NativeHandle> auto &t);

  cpp::NnUp<web::json::value> json_;
};
}  // namespace stonks::network

#endif  // STONKS_RESTSDK_RESTSDK_JSON_NATIVE_HANDLE_H_
