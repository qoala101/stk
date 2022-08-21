#ifndef STONKS_AWS_AWS_API_H_
#define STONKS_AWS_AWS_API_H_

#include "ccutils_not_null.h"

namespace stonks::aws {
/**
 * @brief AWS API lifetime handle.
 * All AWS clients should keep an instance for API to work.
 */
class AwsApi {
 public:
  static auto Instance() -> ccutils::Sp<AwsApi>;

  AwsApi(const AwsApi &) = delete;
  AwsApi(AwsApi &&) noexcept = delete;

  auto operator=(const AwsApi &) -> AwsApi & = delete;
  auto operator=(AwsApi &&) noexcept -> AwsApi & = delete;

  /**
   * @brief Shuts the API down.
   */
  ~AwsApi();

 protected:
  /**
   * @brief Initializes the API.
   */
  explicit AwsApi();
};
}  // namespace stonks::aws

#endif  // STONKS_AWS_AWS_API_H_
