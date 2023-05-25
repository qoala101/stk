/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_I_JSON_H_
#define VH_NETWORK_I_JSON_H_

#include <string>
#include <string_view>

#include "cpp_not_null.h"
#include "cpp_polymorphic_value.h"

namespace vh::network {
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
  virtual auto clone [[nodiscard]] () const -> cpp::NnUp<IJson> = 0;
  virtual auto clone [[nodiscard]] () -> cpp::NnUp<IJson> = 0;

  virtual ~IJson() = default;

  /**
   * @brief Whether JSON is default-created and empty.
   */
  virtual auto IsNull [[nodiscard]] () const -> bool = 0;

  /**
   * @brief Gives child JSON at the key.
   */
  virtual auto GetChild [[nodiscard]] (std::string_view key) const
      -> cpp::Pv<IJson> = 0;
  virtual auto GetChild [[nodiscard]] (std::string_view key)
  -> cpp::Pv<IJson> = 0;

  /**
   * @brief Sets child JSON at the key.
   * @remark Would override existing child at the same key.
   */
  virtual void SetChild(std::string key, cpp::Pv<IJson> child) = 0;

  /**
   * @brief Gives child JSON at index.
   */
  virtual auto GetChild [[nodiscard]] (int index) const -> cpp::Pv<IJson> = 0;
  virtual auto GetChild [[nodiscard]] (int index) -> cpp::Pv<IJson> = 0;

  /**
   * @brief Sets child JSON at index.
   * @remark Would override existing child at the same index.
   */
  virtual void SetChild(int index, cpp::Pv<IJson> child) = 0;

  /**
   * @brief Gives number of children.
   */
  virtual auto GetSize [[nodiscard]] () const -> int = 0;

  /**
   * @brief Gives implementation details.
   */
  virtual auto GetNativeHandle [[nodiscard]] () const
      -> const NativeHandle& = 0;
  virtual auto GetNativeHandle [[nodiscard]] () -> NativeHandle& = 0;
};
}  // namespace vh::network

#endif  // VH_NETWORK_I_JSON_H_
