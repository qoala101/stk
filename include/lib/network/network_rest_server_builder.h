/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_REST_SERVER_BUILDER_H_
#define VH_NETWORK_REST_SERVER_BUILDER_H_

#include <absl/container/flat_hash_map.h>
#include <absl/hash/hash.h>

#include <concepts>
#include <utility>

#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "cpp_smart_pointers.h"
#include "network_auto_parsable_request_handler.h"
#include "network_i_rest_request_handler.h"
#include "network_i_rest_request_receiver.h"
#include "network_rest_server.h"
#include "network_typed_endpoint.h"
#include "network_types.h"

namespace vh::network {
/**
 * @brief Convenience API for building server-like REST request listener.
 */
class RestServerBuilder {
 public:
  /**
   * @param base_uri Base URI on which requests are to be handled.
   */
  RestServerBuilder(Uri base_uri,
                    cpp::NnUp<IRestRequestReceiver> request_receiver);

  /**
   * @brief Registers handler for particular endpoint.
   */
  template <typename Handler>
    requires std::constructible_from<AutoParsableRequestHandler, Handler>
  auto Handling(TypedEndpoint endpoint, Handler &&handler) -> auto & {
    return Handling(std::move(endpoint),
                    cpp::MakeNnUp<AutoParsableRequestHandler>(
                        std::forward<Handler>(handler)));
  }

  /**
   * @brief Starts the server and returns the handle which keeps it alive.
   * @return Keeps handling REST requests while alive.
   * @remark Other methods should not be called after this.
   */
  auto Start [[nodiscard]] () -> RestServer;

 private:
  auto Handling [[nodiscard]] (TypedEndpoint endpoint,
                               cpp::NnUp<IRestRequestHandler> handler)
  -> RestServerBuilder &;

  cpp::Opt<Uri> base_uri_{};
  cpp::Up<IRestRequestReceiver> request_receiver_{};
  absl::flat_hash_map<Endpoint, cpp::NnUp<IRestRequestHandler>>
      endpoint_handlers_{};
};
}  // namespace vh::network

#endif  // VH_NETWORK_REST_SERVER_BUILDER_H_
