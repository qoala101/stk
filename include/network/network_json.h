#ifndef STONKS_NETWORK_NETWORK_JSON_H_
#define STONKS_NETWORK_NETWORK_JSON_H_

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
   * @brief Private implementation to be provided by the client.
   * @remark Can be used by other client entities to access the
   * implementation details.
   */
  class Impl;

  /**
   * @brief Creates a copy with the same data.
   */
  [[nodiscard]] virtual auto Clone() const
      -> cpp::not_null<std::unique_ptr<IJson>> = 0;

  virtual ~IJson() = default;

  [[nodiscard]] virtual auto AsInt() const -> int = 0;
  virtual void SetInt(int value) = 0;

  [[nodiscard]] virtual auto AsInt64() const -> int64_t = 0;
  virtual void SetInt64(int64_t value) = 0;

  [[nodiscard]] virtual auto AsDouble() const -> double = 0;
  virtual void SetDouble(double value) = 0;

  [[nodiscard]] virtual auto AsString() const -> std::string = 0;
  virtual void SetString(std::string_view value) = 0;

  [[nodiscard]] virtual auto GetChild(std::string_view key) const
      -> cpp::not_null<std::unique_ptr<IJson>> = 0;
  virtual void SetChild(std::string_view key, const IJson& child) = 0;

  /**
   * @brief Gives implementation details.
   */
  [[nodiscard]] virtual auto GetImpl() const -> const Impl& = 0;
};

template <typename T>
auto ParseFromJson(const IJson& json) -> T;

template <typename T>
auto ConvertToJson(const T& value) -> cpp::not_null<std::unique_ptr<IJson>>;
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_JSON_H_
