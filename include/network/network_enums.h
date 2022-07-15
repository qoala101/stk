#ifndef STONKS_NETWORK_NETWORK_ENUMS_H_
#define STONKS_NETWORK_NETWORK_ENUMS_H_

namespace stonks::network {
/**
 * @brief REST request method.
 */
enum class Method { kGet, kPost, kDelete, kOther };

/**
 * @brief REST response status.
 */
enum class Status {
  kOtherInfo,
  kOtherSuccess,
  kOk,
  kOtherRedirection,
  kOtherClientError,
  kBadRequest,
  kNotFound,
  kOtherServerError,
  kInternalError
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_ENUMS_H_
