#ifndef STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_H_
#define STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_H_

#include <utility>

#include "network_aprh_concepts.h"  // IWYU pragma: keep
#include "network_aprh_handler_variant.h"
#include "network_auto_parsable_request.h"
#include "network_i_rest_request_handler.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Convenient request handler constructible from any callable which may
 * take auto-parsable request and return any convertible value.
 */
class AutoParsableRequestHandler : public IRestRequestHandler {
 public:
  explicit AutoParsableRequestHandler(aprh::Callable auto handler)
      : handler_{std::in_place_type_t<aprh::Handler>{}, std::move(handler)} {}

  explicit AutoParsableRequestHandler(aprh::CallableWithRequest auto handler)
      : handler_{std::in_place_type_t<aprh::HandlerWithRequest>{},
                 std::move(handler)} {}

  explicit AutoParsableRequestHandler(aprh::CallableWithResponse auto handler)
      : handler_{std::in_place_type_t<aprh::HandlerWithResponse>{},
                 [handler = std::move(handler)]() {
                   return ConvertToJson(handler());
                 }} {}

  explicit AutoParsableRequestHandler(
      aprh::CallableWithRequestAndResponse auto handler)
      : handler_{
            std::in_place_type_t<aprh::HandlerWithRequestAndResponse>{},
            [handler = std::move(handler)](AutoParsableRestRequest request) {
              return ConvertToJson(handler(std::move(request)));
            }} {}

  /**
   * @brief Wraps request in auto-convertible and forwards it to the handler.
   */
  [[nodiscard]] auto HandleRequestAndGiveResponse(RestRequest request) const
      -> RestResponse override;

 private:
  mutable aprh::HandlerVariant handler_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_H_
