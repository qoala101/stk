#include "endpoint.h"

namespace stonks::network {
auto EndpointHandler::operator()(std::map<std::string, std::any> params,
                                 std::any request_body) const -> std::any {
  return std::visit(
      [&params, &request_body](const auto &variant) -> std::any {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, std::function<void()>>) {
          variant();
          return {};
        }

        if constexpr (std::is_same_v<T,
                                     std::function<void(
                                         std::map<std::string, std::any>)>>) {
          variant(std::move(params));
          return {};
        }

        if constexpr (std::is_same_v<T, std::function<void(std::any)>>) {
          variant(std::move(request_body));
          return {};
        }

        if constexpr (std::is_same_v<
                          T, std::function<void(std::map<std::string, std::any>,
                                                std::any)>>) {
          variant(std::move(params), std::move(request_body));
          return {};
        }

        if constexpr (std::is_same_v<T, std::function<std::any()>>) {
          return variant();
        }

        if constexpr (std::is_same_v<T,
                                     std::function<std::any(
                                         std::map<std::string, std::any>)>>) {
          return variant(std::move(params));
        }

        if constexpr (std::is_same_v<T, std::function<std::any(std::any)>>) {
          return variant(std::move(request_body));
        }

        if constexpr (std::is_same_v<
                          T, std::function<std::any(
                                 std::map<std::string, std::any>, std::any)>>) {
          return variant(std::move(params), std::move(request_body));
        }

        return {};
      },
      handler_);
}
}  // namespace stonks::network