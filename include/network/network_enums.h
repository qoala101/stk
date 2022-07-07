#ifndef STONKS_NETWORK_METHOD_H_
#define STONKS_NETWORK_METHOD_H_

namespace stonks::network {
/**
 * @brief REST request method.
 */
enum class Method {
  kGet,
  kPost,
  kPut,
  kDelete,
  kHead,
  kOptions,
  kTrace,
  kConnect,
  kMerge,
  kPatch,
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_METHOD_H_
