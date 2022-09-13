#ifndef STONKS_NETWORK_NETWORK_REST_SERVER_H_
#define STONKS_NETWORK_NETWORK_REST_SERVER_H_

#include <map>
#include <string>
#include <utility>

#include "cpp_factory.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "network_auto_parsable_request_handler.h"
#include "network_i_rest_request_handler.h"
#include "network_i_rest_request_receiver.h"
#include "network_typed_endpoint.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Convenience API for building server-like REST request listener.
 */
class RestServer {
 public:
  explicit RestServer(
      cpp::NnSp<cpp::IFactory<IRestRequestReceiver>> request_receiver_factory);

  /**
   * @brief Sets base URI on which requests are to be handled.
   */
  auto On(std::string base_uri) -> RestServer &;

  /**
   * @brief Registers handler for particular endpoint.
   */
  template <typename T>
  requires std::constructible_from<AutoParsableRequestHandler, T>
  auto Handling(TypedEndpoint endpoint, T handler) -> RestServer & {
    return Handling(
        std::move(endpoint),
        cpp::MakeNnUp<AutoParsableRequestHandler>(std::move(handler)));
  }

  /**
   * @brief Starts the server and returns request receiver
   * responsible for handling of requests.
   * @return Keeps handling REST requests while alive.
   * @remark Other methods should not be called after this.
   */
  [[nodiscard]] auto Start() -> cpp::NnUp<IRestRequestReceiver>;

 private:
  [[nodiscard]] auto Handling(TypedEndpoint endpoint,
                              cpp::NnUp<IRestRequestHandler> handler)
      -> RestServer &;

  cpp::NnSp<cpp::IFactory<IRestRequestReceiver>> request_receiver_factory_;
  cpp::Opt<std::string> base_uri_{};
  std::map<Endpoint, cpp::NnUp<IRestRequestHandler>> endpoint_handlers_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_SERVER_H_
