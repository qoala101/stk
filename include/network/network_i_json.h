#ifndef STONKS_NETWORK_NETWORK_JSON_H_
#define STONKS_NETWORK_NETWORK_JSON_H_

#include <polymorphic_value.h>

#include <cstdint>
#include <exception>
#include <memory>

#include "not_null.hpp"

namespace stonks::network {
/**
 * @brief JSON object.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IJson {
 public:
  /**
   * @brief Private implementation to be provided by the library implementer.
   * @remark Can be used by other implementer entities to access the
   * implementation details.
   */
  class Impl;

  /**
   * @brief Creates a copy with the same data.
   * @remark Required by polymorphic_value.
   */
  [[nodiscard]] virtual auto clone() const
      -> cpp::not_null<std::unique_ptr<IJson>> = 0;

  virtual ~IJson() = default;

  /**
   * @brief Gives child JSON at the key.
   */
  [[nodiscard]] virtual auto GetChild(std::string_view key) const
      -> isocpp_p0201::polymorphic_value<IJson> = 0;

  /**
   * @brief Sets child JSON at the key.
   * @remark Would override existing child at the same key.
   */
  virtual void SetChild(std::string_view key, const IJson &child) = 0;

  /**
   * @brief Gives implementation details.
   */
  [[nodiscard]] virtual auto GetImpl() const -> const Impl & = 0;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_JSON_H_
