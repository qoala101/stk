#include "network_aprh_handler_variant.h"

#include <gsl/assert>
#include <type_traits>
#include <utility>
#include <variant>

#include "network_auto_parsable_request.h"
#include "network_enums.h"
#include "network_types.h"

namespace stonks::network::aprh {
auto HandlerVariant::operator()(RestRequest request) -> RestResponse {
  return std::visit(
      [&request](auto &v) {
        Expects(!v.empty());

        using V = std::decay_t<decltype(v)>;

        if constexpr (std::is_same_v<V, Handler>) {
          v();
          return RestResponse{.status = Status::kOk};
        } else if constexpr (std::is_same_v<V, HandlerWithRequest>) {
          v(AutoParsableRestRequest{std::move(request)});
          return RestResponse{.status = Status::kOk};
        } else if constexpr (std::is_same_v<V, HandlerWithResponse>) {
          return RestResponse{.status = Status::kOk, .result = v()};
        } else if constexpr (std::is_same_v<V, HandlerWithRequestAndResponse>) {
          return RestResponse{
              .status = Status::kOk,
              .result = v(AutoParsableRestRequest{std::move(request)})};
        } else {
          Expects(false);
        }
      },
      value);
}
}  // namespace stonks::network::aprh