#ifndef STONKS_RESTSDK_RESTSDK_JSON_IMPL_H_
#define STONKS_RESTSDK_RESTSDK_JSON_IMPL_H_

#include <memory>

#include "network_json.h"
#include "not_null.hpp"

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
  explicit Impl(const web::json::value &json);
  explicit Impl(web::json::value &&json);

  Impl(const Impl &other);
  Impl(Impl &&) noexcept;

  auto operator=(const Impl &other) -> Impl &;
  auto operator=(Impl &&) noexcept -> Impl &;

  ~Impl();

  [[nodiscard]] auto GetJson() const -> const web::json::value &;
  [[nodiscard]] auto GetJson() -> web::json::value &;

 private:
  template <typename T>
  [[nodiscard]] static auto GetJsonImpl(T &t) -> auto &;

  cpp::not_null<std::unique_ptr<web::json::value>> json_;
};
}  // namespace stonks::network

#endif  // STONKS_RESTSDK_RESTSDK_JSON_IMPL_H_
