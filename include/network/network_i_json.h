#ifndef STONKS_NETWORK_NETWORK_I_JSON_H_
#define STONKS_NETWORK_NETWORK_I_JSON_H_

#include <string>
#include <string_view>

#include "ccutils_not_null.h"
#include "ccutils_polymorphic_value.h"

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
  [[nodiscard]] virtual auto clone() const -> ccutils::NnUp<IJson> = 0;

  virtual ~IJson() = default;

  /**
   * @brief Gives child JSON at the key.
   */
  [[nodiscard]] virtual auto GetChild(
      std::string_view key) const& -> ccutils::Pv<IJson> = 0;

  /**
   * @copydoc GetChild
   */
  [[nodiscard]] virtual auto GetChild(
      std::string_view key) && -> ccutils::Pv<IJson> = 0;

  /**
   * @brief Sets child JSON at the key.
   * @remark Would override existing child at the same key.
   */
  virtual void SetChild(std::string key, const IJson& child) = 0;

  /**
   * @copydoc SetChild
   */
  virtual void SetChild(std::string key, IJson&& child);

  /**
   * @brief Gives implementation details.
   */
  [[nodiscard]] virtual auto GetImpl() const -> const Impl& = 0;

  /**
   * @copydoc GetImpl
   */
  [[nodiscard]] virtual auto GetImpl() -> Impl& = 0;

 protected:
  explicit IJson() = default;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_I_JSON_H_
