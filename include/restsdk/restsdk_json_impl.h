#ifndef STONKS_RESTSDK_RESTSDK_JSON_IMPL_H_
#define STONKS_RESTSDK_RESTSDK_JSON_IMPL_H_

#include <utility>

#include "cpp_not_null.h"
#include "network_i_json.h"

namespace web::json {
class value;
}  // namespace web::json

namespace stonks::network {
/**
 * @brief JSON implementation details.
 */
class IJson::Impl {
 public:
  explicit Impl();
  explicit Impl(auto &&...ts)
      : Impl{web::json::value{std::forward<decltype(ts)>(ts)...}} {}

  Impl(const Impl &other);
  Impl(Impl &&) noexcept;

  auto operator=(const Impl &other) -> Impl &;
  auto operator=(Impl &&) noexcept -> Impl &;

  ~Impl();

  /**
   * @brief Get native handle.
   */
  [[nodiscard]] auto GetJson() const -> const web::json::value &;

  /**
   * @copydoc GetJson
   */
  [[nodiscard]] auto GetJson() -> web::json::value &;

 private:
  cpp::NnUp<web::json::value> json_;
};
}  // namespace stonks::network

#endif  // STONKS_RESTSDK_RESTSDK_JSON_IMPL_H_
