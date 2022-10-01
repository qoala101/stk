#ifndef STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_H_
#define STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_H_

#include <utility>

#include "cpp_typed_struct.h"
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
  template <cpp::VoidInvocable T>
  // NOLINTNEXTLINE(*-forwarding-reference-overload)
  explicit AutoParsableRequestHandler(T &&handler)
      : handler_{aprh::HandlerVariant::ValueType{
            std::in_place_type_t<aprh::Handler>{}, std::forward<T>(handler)}} {}

  template <cpp::VoidInvocableTakes<AutoParsableRestRequest> T>
  // NOLINTNEXTLINE(*-forwarding-reference-overload)
  explicit AutoParsableRequestHandler(T &&handler)
      : handler_{aprh::HandlerVariant::ValueType{
            std::in_place_type_t<aprh::HandlerWithRequest>{},
            std::forward<T>(handler)}} {}

  template <aprh::ConvertibleInvocable T>
  // NOLINTNEXTLINE(*-forwarding-reference-overload)
  explicit AutoParsableRequestHandler(T &&handler)
      : handler_{aprh::HandlerVariant::ValueType{
            std::in_place_type_t<aprh::HandlerWithResponse>{},
            [handler = std::forward<T>(handler)]() {
              return ConvertToJson(handler());
            }}} {}

  template <aprh::ConvertibleInvocableTakes<AutoParsableRestRequest> T>
  // NOLINTNEXTLINE(*-forwarding-reference-overload)
  explicit AutoParsableRequestHandler(T &&handler)
      : handler_{aprh::HandlerVariant::ValueType{
            std::in_place_type_t<aprh::HandlerWithRequestAndResponse>{},
            [handler =
                 std::forward<T>(handler)](AutoParsableRestRequest request) {
              return ConvertToJson(handler(std::move(request)));
            }}} {}

  /**
   * @brief Wraps request in auto-parsable and forwards it to the handler.
   */
  [[nodiscard]] auto HandleRequestAndGiveResponse(RestRequest request) const
      -> RestResponse override;

 private:
  mutable aprh::HandlerVariant handler_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_H_
