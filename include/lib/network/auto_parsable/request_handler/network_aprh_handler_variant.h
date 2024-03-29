/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_APRH_HANDLER_VARIANT_H_
#define VH_NETWORK_APRH_HANDLER_VARIANT_H_

#include <cppcoro/task.hpp>
#include <function2/function2.hpp>

#include "cpp_variant_struct.h"
#include "network_auto_parsable_request.h"
#include "network_types.h"

namespace vh::network::aprh {
using Handler = fu2::unique_function<auto()->cppcoro::task<>>;
using HandlerWithRequest =
    fu2::unique_function<auto(AutoParsableRestRequest)->cppcoro::task<>>;
using HandlerWithResponse = fu2::unique_function<auto()->cppcoro::task<Result>>;
using HandlerWithRequestAndResponse =
    fu2::unique_function<auto(AutoParsableRestRequest)->cppcoro::task<Result>>;

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
}  // namespace vh::network::aprh

#endif  // VH_NETWORK_APRH_HANDLER_VARIANT_H_
