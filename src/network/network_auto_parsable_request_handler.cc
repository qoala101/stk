#include "network_auto_parsable_request_handler.h"

#include <gsl/assert>
#include <optional>
#include <type_traits>

#include "network_auto_parsable_request.h"
#include "network_enums.h"

namespace stonks::network {
auto AutoParsableRequestHandler::HandleRequestAndGiveResponse(
    RestRequest request) const -> RestResponse {
  return std::visit(
      [&request](const auto &variant) -> RestResponse {
        Expects(variant);

        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, Handler>) {
          variant();
          return {Status::kOk, std::nullopt};
        }

        if constexpr (std::is_same_v<T, HandlerWithRequest>) {
          variant(AutoParsableRestRequest{std::move(request)});
          return {Status::kOk, std::nullopt};
        }

        if constexpr (std::is_same_v<T, HandlerWithResponse>) {
          return {Status::kOk, variant()};
        }

        if constexpr (std::is_same_v<T, HandlerWithRequestAndResponse>) {
          return {Status::kOk,
                  variant(AutoParsableRestRequest{std::move(request)})};
        }

        Expects(false);
      },
      handler_);
}
}  // namespace stonks::network