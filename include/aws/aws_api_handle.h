#ifndef STONKS_AWS_AWS_API_HANDLE_H_
#define STONKS_AWS_AWS_API_HANDLE_H_

#include "cpp_not_null.h"

namespace stonks::aws {
/**
 * @brief AWS API lifetime handle.
 * All AWS clients should keep an instance for API to work.
 */
class ApiHandle {
 public:
  /**
   * @brief Gives shared instance.
   */
  [[nodiscard]] static auto Instance() -> cpp::NnSp<ApiHandle>;

  ApiHandle(const ApiHandle &) = delete;
  ApiHandle(ApiHandle &&) noexcept = delete;

  auto operator=(const ApiHandle &) -> ApiHandle & = delete;
  auto operator=(ApiHandle &&) noexcept -> ApiHandle & = delete;

  /**
   * @brief Shuts the API down.
   */
  ~ApiHandle();

 private:
  /**
   * @brief Initializes the API.
   */
  explicit ApiHandle();
};
}  // namespace stonks::aws

#endif  // STONKS_AWS_AWS_API_HANDLE_H_
