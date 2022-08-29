#ifndef STONKS_AWS_AWS_API_H_
#define STONKS_AWS_AWS_API_H_

#include "cpp_smart_pointers.h"

namespace stonks::aws {
/**
 * @brief AWS API lifetime handle.
 * All AWS clients should keep an instance for API to work.
 */
class ApiHandle {
 public:
  static auto Instance() -> cpp::Sp<ApiHandle>;

  ApiHandle(const ApiHandle &) = delete;
  ApiHandle(ApiHandle &&) noexcept = delete;

  auto operator=(const ApiHandle &) -> ApiHandle & = delete;
  auto operator=(ApiHandle &&) noexcept -> ApiHandle & = delete;

  /**
   * @brief Shuts the API down.
   */
  ~ApiHandle();

 protected:
  /**
   * @brief Initializes the API.
   */
  explicit ApiHandle();
};
}  // namespace stonks::aws

#endif  // STONKS_AWS_AWS_API_H_
