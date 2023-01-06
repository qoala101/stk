#ifndef STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_H_
#define STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_H_

#include <cppcoro/task.hpp>
#include <utility>
#include <variant>

#include "cpp_polymorphic_value.h"
#include "network_aprh_concepts.h"  // IWYU pragma: keep
#include "network_aprh_handler_variant.h"
#include "network_auto_parsable_request.h"
#include "network_i_json.h"
#include "network_i_rest_request_handler.h"
#include "network_json_base_conversions.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Convenient request handler constructible from any callable which may
 * take auto-parsable request and return any convertible value.
 */
class AutoParsableRequestHandler : public IRestRequestHandler {
 public:
  template <aprh::VoidInvocable Handler>
  // NOLINTNEXTLINE(*-forwarding-reference-overload)
  explicit AutoParsableRequestHandler(Handler &&handler)
      : handler_{
            aprh::HandlerVariant::ValueType{std::in_place_type<aprh::Handler>,
                                            std::forward<Handler>(handler)}} {}

  template <aprh::VoidInvocableTaking<AutoParsableRestRequest> Handler>
  // NOLINTNEXTLINE(*-forwarding-reference-overload)
  explicit AutoParsableRequestHandler(Handler &&handler)
      : handler_{aprh::HandlerVariant::ValueType{
            std::in_place_type<aprh::HandlerWithRequest>,
            std::forward<Handler>(handler)}} {}

  template <aprh::ConvertibleInvocable Handler>
  // NOLINTNEXTLINE(*-forwarding-reference-overload)
  explicit AutoParsableRequestHandler(Handler &&handler)
      : handler_{aprh::HandlerVariant::ValueType{
            std::in_place_type<aprh::HandlerWithResponse>,
            [handler = std::forward<Handler>(
                 handler)]() -> cppcoro::task<cpp::Pv<IJson>> {
              co_return ConvertToJson(co_await handler());
            }}} {}

  template <aprh::ConvertibleInvocableTaking<AutoParsableRestRequest> Handler>
  // NOLINTNEXTLINE(*-forwarding-reference-overload)
  explicit AutoParsableRequestHandler(Handler &&handler)
      : handler_{aprh::HandlerVariant::ValueType{
            std::in_place_type<aprh::HandlerWithRequestAndResponse>,
            [handler = std::forward<Handler>(handler)](
                auto request) -> cppcoro::task<cpp::Pv<IJson>> {
              co_return ConvertToJson(co_await handler(std::move(request)));
            }}} {}

  /**
   * @brief Wraps request in auto-parsable and forwards it to the handler.
   */
  auto HandleRequestAndGiveResponse [[nodiscard]] (RestRequest request)
  -> cppcoro::task<RestResponse> override;

 private:
  aprh::HandlerVariant handler_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_H_
