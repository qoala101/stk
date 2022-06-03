#include "endpoint.h"

#include <type_traits>
#include <utility>

namespace stonks::network {
auto EndpointHandler::operator()(Params params,
                                 Body request_body) const -> Result {
  return std::visit(
      [&params, &request_body](const auto &variant) -> Result {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, NoResult>) {
          variant();
          return {};
        }

        if constexpr (std::is_same_v<T, NoResultTakesParams>) {
          variant(std::move(params));
          return {};
        }

        if constexpr (std::is_same_v<T, NoResultTakesBody>) {
          variant(std::move(request_body));
          return {};
        }

        if constexpr (std::is_same_v<T, NoResultTakesParamsAndBody>) {
          variant(std::move(params), std::move(request_body));
          return {};
        }

        if constexpr (std::is_same_v<T, HasResult>) {
          return variant();
        }

        if constexpr (std::is_same_v<T, HasResultTakesParams>) {
          return variant(std::move(params));
        }

        if constexpr (std::is_same_v<T, HasResultTakesBody>) {
          return variant(std::move(request_body));
        }

        if constexpr (std::is_same_v<T, HasResultTakesParamsAndBody>) {
          return variant(std::move(params), std::move(request_body));
        }

        return {};
      },
      handler_);
}
}  // namespace stonks::network