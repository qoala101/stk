#ifndef STONKS_NETWORK_NETWORK_I_JSON_H_
#define STONKS_NETWORK_NETWORK_I_JSON_H_

#include <string>
#include <string_view>

#include "cpp_not_null.h"
#include "cpp_polymorphic_value.h"

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
  class NativeHandle;

  /**
   * @brief Creates a copy with the same data.
   * @remark Required by the polymorphic_value.
   */
  [[nodiscard]] virtual auto clone() const -> cpp::NnUp<IJson> = 0;

  /**
   * @copydoc clone
   */
  [[nodiscard]] virtual auto clone() -> cpp::NnUp<IJson> = 0;

  virtual ~IJson() = default;

  /**
   * @brief Whether JSON is default-created and empty.
   */
  [[nodiscard]] virtual auto IsNull() const -> bool = 0;

  /**
   * @brief Gives child JSON at the key.
   */
  [[nodiscard]] virtual auto GetChild(std::string_view key) const
      -> cpp::Pv<IJson> = 0;

  /**
   * @copydoc GetChild
   */
  [[nodiscard]] virtual auto GetChild(std::string_view key)
      -> cpp::Pv<IJson> = 0;

  /**
   * @brief Sets child JSON at the key.
   * @remark Would override existing child at the same key.
   */
  virtual void SetChild(std::string key, cpp::Pv<IJson> child) = 0;

  /**
   * @brief Gives child JSON at index.
   */
  [[nodiscard]] virtual auto GetChild(int index) const -> cpp::Pv<IJson> = 0;

  /**
   * @copydoc GetChild
   */
  [[nodiscard]] virtual auto GetChild(int index) -> cpp::Pv<IJson> = 0;

  /**
   * @brief Sets child JSON at index.
   * @remark Would override existing child at the same index.
   */
  virtual void SetChild(int index, cpp::Pv<IJson> child) = 0;

  /**
   * @brief Gives number of children.
   */
  [[nodiscard]] virtual auto GetSize() const -> int = 0;

  /**
   * @brief Gives implementation details.
   */
  [[nodiscard]] virtual auto GetNativeHandle() const -> const NativeHandle& = 0;

  /**
   * @copydoc GetImpl
   */
  [[nodiscard]] virtual auto GetNativeHandle() -> NativeHandle& = 0;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_I_JSON_H_
