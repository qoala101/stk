#ifndef STONKS_NETWORK_NETWORK_REST_REQUEST_RECEIVER_H_
#define STONKS_NETWORK_NETWORK_REST_REQUEST_RECEIVER_H_

namespace stonks::network {
/**
 * @brief Receives REST requests at the specified URI and redirects
 * to the handler.
 * @remark URI and handler are specified during construction.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IRestRequestReceiver {
 public:
  virtual ~IRestRequestReceiver() = default;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_REQUEST_RECEIVER_H_
