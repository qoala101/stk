#include "network_auto_parsable_request_handler.h"

#include <gsl/assert>
#include <optional>
#include <utility>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "network_auto_parsable_request.h"
#include "network_enums.h"

namespace stonks::network {
auto AutoParsableRequestHandler::HandleRequestAndGiveResponse(
    RestRequest request) const -> RestResponse {
  return std::visit(
      [&request](const auto &v) -> RestResponse {
        Expects(v);

        using V = decltype(v);

        if constexpr (cpp::DecaysTo<V, Handler>) {
          v();
          return {Status::kOk, std::nullopt};
        }

        if constexpr (cpp::DecaysTo<V, HandlerWithRequest>) {
          v(AutoParsableRestRequest{std::move(request)});
          return {Status::kOk, std::nullopt};
        }

        if constexpr (cpp::DecaysTo<V, HandlerWithResponse>) {
          return {Status::kOk, v()};
        }

        if constexpr (cpp::DecaysTo<V, HandlerWithRequestAndResponse>) {
          return {Status::kOk, v(AutoParsableRestRequest{std::move(request)})};
        }

        Expects(false);
      },
      handler_);
}
}  // namespace stonks::network