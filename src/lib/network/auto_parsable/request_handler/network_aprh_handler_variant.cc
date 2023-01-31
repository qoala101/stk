#include "network_aprh_handler_variant.h"

#include <coroutine>
#include <gsl/assert>
#include <type_traits>
#include <utility>
#include <variant>

#include "cpp_typed_struct.h"
#include "network_auto_parsable_request.h"
#include "network_types.h"

namespace vh::network::aprh {
auto HandlerVariant::operator()(RestRequest request)
    -> cppcoro::task<RestResponse> {
  co_return co_await std::visit(
      [&request](auto &v) -> cppcoro::task<RestResponse> {
        Expects(!v.empty());

        using V = std::remove_cvref_t<decltype(v)>;

        if constexpr (std::is_same_v<V, Handler>) {
          co_await v();
          co_return RestResponse{.status = Status::kOk};
        } else if constexpr (std::is_same_v<V, HandlerWithRequest>) {
          co_await v(AutoParsableRestRequest{std::move(request)});
          co_return RestResponse{.status = Status::kOk};
        } else if constexpr (std::is_same_v<V, HandlerWithResponse>) {
          co_return RestResponse{.status = Status::kOk, .result = co_await v()};
        } else if constexpr (std::is_same_v<V, HandlerWithRequestAndResponse>) {
          co_return RestResponse{
              .status = Status::kOk,
              .result =
                  co_await v(AutoParsableRestRequest{std::move(request)})};
        } else {
          Expects(false);
        }
      },
      **this);
}
}  // namespace vh::network::aprh