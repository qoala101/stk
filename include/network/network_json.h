#ifndef STONKS_NETWORK_NETWORK_JSON_H_
#define STONKS_NETWORK_NETWORK_JSON_H_

#include <exception>
#include <gsl/pointers>

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

  virtual ~IJson() = default;

  /**
   * @brief Creates a copy with the same data.
   */
  [[nodiscard]] virtual auto Clone() const
      -> cpp::not_null<std::unique_ptr<IJson>> = 0;

  /**
   * @brief Gives implementation details.
   */
  [[nodiscard]] virtual auto GetImpl() const -> const Impl& = 0;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_JSON_H_
