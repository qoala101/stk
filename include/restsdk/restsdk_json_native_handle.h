#ifndef STONKS_RESTSDK_RESTSDK_JSON_NATIVE_HANDLE_H_
#define STONKS_RESTSDK_RESTSDK_JSON_NATIVE_HANDLE_H_

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_not_null.h"
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
  [[nodiscard]] auto operator->() const -> const web::json::value *;

  /**
   * @copydoc operator->
   */
  [[nodiscard]] auto operator->() -> web::json::value *;

  /**
   * @brief Gives REST SDK JSON handle.
   */
  [[nodiscard]] auto operator*() const -> const web::json::value &;

  /**
   * @copydoc operator*
   */
  [[nodiscard]] auto operator*() -> web::json::value &;

 private:
  template <cpp::DecaysTo<NativeHandle> This>
  [[nodiscard]] static auto OperatorAsteriskImpl(This &t) -> auto &;

  cpp::NnUp<web::json::value> json_;
};
}  // namespace stonks::network

#endif  // STONKS_RESTSDK_RESTSDK_JSON_NATIVE_HANDLE_H_
