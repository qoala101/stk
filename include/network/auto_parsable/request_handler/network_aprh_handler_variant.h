#ifndef STONKS_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_NETWORK_APRH_HANDLER_VARIANT_H_
#define STONKS_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_NETWORK_APRH_HANDLER_VARIANT_H_

#include <cppcoro/task.hpp>
#include <function2/function2.hpp>

#include "cpp_variant_struct.h"
#include "network_auto_parsable_request.h"
#include "network_types.h"

namespace stonks::network::aprh {
using Handler = fu2::unique_function<auto() const->cppcoro::task<>>;
using HandlerWithRequest =
    fu2::unique_function<auto(AutoParsableRestRequest) const->cppcoro::task<>>;
using HandlerWithResponse =
    fu2::unique_function<auto() const->cppcoro::task<Result::value_type>>;
using HandlerWithRequestAndResponse =
    fu2::unique_function<auto(AutoParsableRestRequest)
                             const->cppcoro::task<Result::value_type>>;

/**
 * @brief Variant of auto-parsable request handler.
 */
struct HandlerVariant
    : public cpp::VariantStruct<Handler, HandlerWithRequest,
                                HandlerWithResponse,
                                HandlerWithRequestAndResponse> {
  /**
   * @brief Calls operator of the current handler variant.
   */
  auto operator() [[nodiscard]] (RestRequest request)
  -> cppcoro::task<RestResponse>;
};
}  // namespace stonks::network::aprh

#endif  // STONKS_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_NETWORK_APRH_HANDLER_VARIANT_H_
