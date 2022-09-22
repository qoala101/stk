#include "network_aprh_handler_variant.h"

#include <gsl/assert>
#include <utility>

#include "network_auto_parsable_request.h"
#include "network_enums.h"

namespace stonks::network::aprh {
auto HandlerVariant::operator()(RestRequest request) -> RestResponse {
  return std::visit(
      [&request](auto &v) -> RestResponse {
        Expects(v);

        using V = decltype(v);

        if constexpr (cpp::DecaysTo<V, Handler>) {
          v();
          return {.status = Status::kOk};
        }

        if constexpr (cpp::DecaysTo<V, HandlerWithRequest>) {
          v(AutoParsableRestRequest{std::move(request)});
          return {.status = Status::kOk};
        }

        if constexpr (cpp::DecaysTo<V, HandlerWithResponse>) {
          return {.status = Status::kOk, .result = v()};
        }

        if constexpr (cpp::DecaysTo<V, HandlerWithRequestAndResponse>) {
          return {.status = Status::kOk,
                  .result = v(AutoParsableRestRequest{std::move(request)})};
        }

        Expects(false);
      },
      static_cast<detail::HandlerVariantType &>(*this));
}
}  // namespace stonks::network::aprh