#ifndef STONKS_NETWORK_NETWORK_REST_REQUEST_HANDLER_H_
#define STONKS_NETWORK_NETWORK_REST_REQUEST_HANDLER_H_

namespace stonks::network {
/**
 * @brief Handles REST requests.
 * @remark Listened URI is specified during construction.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IRestRequestHandler {
 public:
  virtual ~IRestRequestHandler() = default;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_REQUEST_HANDLER_H_
